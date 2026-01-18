#include "s21_sscanf_internal.h"

#include <math.h>

/*
    Файл отвечает за разбор и чтение чисел с плавающей точкой для sscanf
*/

// Внутреннее состояние разбора числа с плавающей точкой.
// Храним позицию для отката, остаток ширины, знак и факт наличия цифр.
typedef struct {
  const char *in0;
  int consumed0;
  int wleft;
  int sign;
  int have_digit;
} s21_fstate_t;

// Возвращает эффективную ширину для float-разбора: если width не задана, считаем её бесконечной.
static int s21_effective_width_float(int width);

// Откатывает входной контекст к сохранённой позиции (используется при mismatch/невалидной экспоненте и т.п.).
static void s21_restore_ctx_f(s21_scan_ctx_t *ctx, const s21_fstate_t *st);

// Проверяет, можно ли читать ещё один символ из входа с учётом оставшейся ширины.
static int s21_can_read_f(const s21_scan_ctx_t *ctx, int wleft);

// Возвращает текущий символ входа без потребления (0 при некорректном контексте).
static unsigned char s21_peek_f(const s21_scan_ctx_t *ctx);

// Потребляет один символ входа: двигает курсор, увеличивает consumed и уменьшает wleft.
static void s21_consume_f(s21_scan_ctx_t *ctx, int *wleft);

// Сопоставляет ключевое слово kw с входом без учёта регистра в пределах wleft; возвращает длину совпадения.
static int s21_match_ci_kw(const char *in, int wleft, const char *kw, int *klen);

// Пытается разобрать специальные значения nan/inf/infinity (без учёта регистра); при успехе пишет *out.
static int s21_try_parse_special(s21_scan_ctx_t *ctx, s21_fstate_t *st, long double *out);

// Читает опциональный знак '+'/'-' и сохраняет его в st->sign.
static void s21_parse_sign_f(s21_scan_ctx_t *ctx, s21_fstate_t *st);

// Читает последовательность десятичных цифр (до переполнения ULLONG_MAX с насыщением), возвращает значение и количество цифр.
static unsigned long long s21_parse_digits_u64(s21_scan_ctx_t *ctx, int *wleft, int *nd);

// Возвращает 10^exp в формате long double (для масштаба дробной части и экспоненты).
static long double s21_pow10_ld(int exp);

// Пытается разобрать экспоненту вида e[+/-]digits; при неудаче откатывает позицию и ширину.
static int s21_try_parse_exponent(s21_scan_ctx_t *ctx, int *wleft, int *exp10);

// Разбирает обычное число с плавающей точкой (не nan/inf): целая часть, дробная часть, экспонента.
static s21_scan_status_t s21_read_float_regular(s21_scan_ctx_t *ctx, s21_fstate_t *st, long double *out);

// Ядро разбора float по правилам scanf: [sign] (digits[.digits] | .digits) [e/E [sign] digits], плюс nan/inf.
// Возвращает статус (OK / MATCH_FAIL / INPUT_FAIL) и при успехе пишет значение в *out.
s21_scan_status_t s21_read_float_core(s21_scan_ctx_t *ctx, int width, long double *out) {
  s21_scan_status_t res = S21_SCAN_MATCH_FAIL;

  if (ctx == NULL || ctx->in == NULL || out == NULL) {
    res = S21_SCAN_MATCH_FAIL;
  } else {
    s21_fstate_t st;

    st.in0 = ctx->in;
    st.consumed0 = ctx->consumed;
    st.wleft = s21_effective_width_float(width);
    st.sign = 1;
    st.have_digit = 0;

    if (!s21_can_read_f(ctx, st.wleft)) {
      ctx->input_fail = 1;
      res = S21_SCAN_INPUT_FAIL;
    } else {
      s21_parse_sign_f(ctx, &st);

      if (s21_try_parse_special(ctx, &st, out)) {
        *out = (long double)st.sign * (*out);
        res = S21_SCAN_OK;
      } else {
        res = s21_read_float_regular(ctx, &st, out);
      }
    }
  }

  return res;
}

static s21_scan_status_t s21_read_float_regular(s21_scan_ctx_t *ctx,
                                                s21_fstate_t *st,
                                                long double *out) {
  s21_scan_status_t res = S21_SCAN_MATCH_FAIL;
  int nd_int = 0;
  int nd_frac = 0;
  unsigned long long int_part = 0ULL;
  unsigned long long frac_part = 0ULL;
  int exp10 = 0;
  int exp_part = 0;
  long double v = 0.0L;

  int_part = s21_parse_digits_u64(ctx, &st->wleft, &nd_int);
  st->have_digit = (nd_int > 0);

  if (s21_can_read_f(ctx, st->wleft) && s21_peek_f(ctx) == (unsigned char)'.') {
    s21_consume_f(ctx, &st->wleft);
    frac_part = s21_parse_digits_u64(ctx, &st->wleft, &nd_frac);
    if (nd_frac > 0) st->have_digit = 1;
  }

  if (st->have_digit) {
    exp10 = 0;
    if (s21_try_parse_exponent(ctx, &st->wleft, &exp_part)) exp10 = exp_part;

    v = (long double)int_part;
    if (nd_frac > 0) v += (long double)frac_part * s21_pow10_ld(-nd_frac);
    v *= s21_pow10_ld(exp10);
    *out = (long double)st->sign * v;
    res = S21_SCAN_OK;
  } else {
    s21_restore_ctx_f(ctx, st);
    res = (*st->in0 == '\0') ? S21_SCAN_INPUT_FAIL : S21_SCAN_MATCH_FAIL;
    if (res == S21_SCAN_INPUT_FAIL) ctx->input_fail = 1;
  }

  return res;
}

// Обёртка для спецификаторов %f/%e/%g (и их варианты с длиной): пропускает пробелы, читает число и пишет его в аргумент (если не подавлено '*').
s21_scan_status_t s21_scan_float(s21_scan_ctx_t *ctx, const s21_fmt_token_t *t) {
  s21_scan_status_t res = S21_SCAN_MATCH_FAIL;

  if (ctx == NULL || t == NULL) {
    res = S21_SCAN_MATCH_FAIL;
  } else {
    long double v = 0.0L;

    s21_skip_spaces_in(ctx);
    res = s21_read_float_core(ctx, t->width, &v);
    if (res == S21_SCAN_OK) {
      if (!t->suppress && ctx->ap != NULL) {
        s21_store_float(ctx->ap, t->len, v);
      }
    }
  }

  return res;
}

/* вспомогательные функции */

// Возвращает эффективную ширину для float-разбора: если width не задана, используем INT_MAX.
static int s21_effective_width_float(int width) {
  int w = width;
  if (w <= 0) w = INT_MAX;
  return w;
}

// Откатывает курсор входа и счётчик consumed к сохранённым значениям.
static void s21_restore_ctx_f(s21_scan_ctx_t *ctx, const s21_fstate_t *st) {
  if (ctx != NULL && st != NULL) {
    ctx->in = st->in0;
    ctx->consumed = st->consumed0;
  }
}

// Проверяет возможность чтения: ctx валиден, ширина не исчерпана и не достигнут '\0'.
static int s21_can_read_f(const s21_scan_ctx_t *ctx, int wleft) {
  int ok = 0;
  if (ctx != NULL && ctx->in != NULL && wleft > 0 && *ctx->in != '\0') ok = 1;
  return ok;
}

// Возвращает текущий символ входа без потребления.
static unsigned char s21_peek_f(const s21_scan_ctx_t *ctx) {
  unsigned char c = 0U;
  if (ctx != NULL && ctx->in != NULL) c = (unsigned char)(*ctx->in);
  return c;
}

// Потребляет один символ входа и обновляет ctx и wleft
static void s21_consume_f(s21_scan_ctx_t *ctx, int *wleft) {
  if (ctx != NULL && wleft != NULL && *wleft > 0 && ctx->in != NULL && *ctx->in != '\0') {
    ctx->in++;
    ctx->consumed++;
    (*wleft)--;
  }
}

// Разбирает и запоминает знак числа; знак не считается "цифрой", но влияет на итоговое значение.
static void s21_parse_sign_f(s21_scan_ctx_t *ctx, s21_fstate_t *st) {
  if (ctx != NULL && st != NULL && s21_can_read_f(ctx, st->wleft)) {
    unsigned char c = s21_peek_f(ctx);
    if (c == (unsigned char)'+' || c == (unsigned char)'-') {
      if (c == (unsigned char)'-') st->sign = -1;
      s21_consume_f(ctx, &st->wleft);
    }
  }
}

// Читает подряд десятичные цифры и накапливает их в unsigned long long.
// При переполнении выполняет насыщение до ULLONG_MAX, чтобы избежать UB.
static unsigned long long s21_parse_digits_u64(s21_scan_ctx_t *ctx, int *wleft, int *nd) {
  unsigned long long acc = 0ULL;
  int count = 0;

  if (ctx != NULL && wleft != NULL && nd != NULL) {
    while (s21_can_read_f(ctx, *wleft) && s21_is_digit(s21_peek_f(ctx))) {
      unsigned char c = s21_peek_f(ctx);
      int digit = (int)(c - (unsigned char)'0');

      if (acc > (ULLONG_MAX - (unsigned long long)digit) / 10ULL) {
        acc = ULLONG_MAX;
      } else {
        acc = acc * 10ULL + (unsigned long long)digit;
      }

      s21_consume_f(ctx, wleft);
      count++;
    }
    *nd = count;
  }

  return acc;
}

// Вычисляет 10^exp через умножение/деление на крупные ступени (1e16/1e8/1e4/10), чтобы не гонять циклы по одной степени.
static long double s21_pow10_ld(int exp) {
  long double r = 1.0L;
  int e = exp;
  int neg = 0;

  if (e < 0) {
    neg = 1;
    e = -e;
  }

  while (e > 0) {
    if (e >= 16) {
      r *= 1.0e16L;
      e -= 16;
    } else if (e >= 8) {
      r *= 1.0e8L;
      e -= 8;
    } else if (e >= 4) {
      r *= 1.0e4L;
      e -= 4;
    } else {
      r *= 10.0L;
      e -= 1;
    }
  }

  if (neg) r = 1.0L / r;
  return r;
}

// Пытается разобрать экспоненту: 'e'/'E' затем необязательный знак и хотя бы одна цифра.
// Если после 'e' не нашлось цифр, откатывает ctx и wleft назад, как будто экспоненты не было.
static int s21_try_parse_exponent(s21_scan_ctx_t *ctx, int *wleft, int *exp10) {
  int ok = 0;

  if (ctx != NULL && wleft != NULL && exp10 != NULL) {
    const char *in0 = ctx->in;
    int consumed0 = ctx->consumed;
    int w0 = *wleft;

    if (s21_can_read_f(ctx, *wleft)) {
      unsigned char c = s21_peek_f(ctx);
      if (c == (unsigned char)'e' || c == (unsigned char)'E') {
        int esign = 1;
        int nd = 0;
        int ev = 0;

        s21_consume_f(ctx, wleft);

        if (s21_can_read_f(ctx, *wleft)) {
          unsigned char s = s21_peek_f(ctx);
          if (s == (unsigned char)'+' || s == (unsigned char)'-') {
            if (s == (unsigned char)'-') esign = -1;
            s21_consume_f(ctx, wleft);
          }
        }

        while (s21_can_read_f(ctx, *wleft) && s21_is_digit(s21_peek_f(ctx))) {
          int digit = (int)(s21_peek_f(ctx) - (unsigned char)'0');
          if (ev <= (INT_MAX - digit) / 10) ev = ev * 10 + digit;
          s21_consume_f(ctx, wleft);
          nd++;
        }

        if (nd > 0) {
          *exp10 = esign * ev;
          ok = 1;
        } else {
          ctx->in = in0;
          ctx->consumed = consumed0;
          *wleft = w0;
        }
      }
    }
  }

  return ok;
}

// Сравнивает вход с ключевым словом без учёта регистра в пределах wleft; на успех возвращает длину совпадения.
static int s21_match_ci_kw(const char *in, int wleft, const char *kw, int *klen) {
  int ok = 0;
  int i = 0;

  if (in != NULL && kw != NULL && klen != NULL) {
    while (kw[i] != '\0' && i < wleft) {
      unsigned char a = (unsigned char)kw[i];
      unsigned char b = (unsigned char)in[i];
      if (b == '\0') break;

      a = s21_tolower_ascii(a);
      b = s21_tolower_ascii(b);

      if (a != b) break;
      i++;
    }

    if (kw[i] == '\0') {
      ok = 1;
      *klen = i;
    } else {
      *klen = 0;
    }
  }

  return ok;
}

// Пытается разобрать специальные значения: nan, nan(...), inf, infinity (без учёта регистра).
// При успехе потребляет соответствующие символы из входа и выставляет *out.
static int s21_try_parse_special(s21_scan_ctx_t *ctx, s21_fstate_t *st, long double *out) {
  int ok = 0;

  if (ctx != NULL && st != NULL && out != NULL) {
    int klen = 0;

    if (s21_match_ci_kw(ctx->in, st->wleft, "nan", &klen)) {
      int n = klen;
      *out = NAN;
      while (n > 0) {
        s21_consume_f(ctx, &st->wleft);
        n--;
      }

      if (s21_can_read_f(ctx, st->wleft) && s21_peek_f(ctx) == (unsigned char)'(') {
        s21_consume_f(ctx, &st->wleft);
        while (s21_can_read_f(ctx, st->wleft) && s21_peek_f(ctx) != (unsigned char)')') {
          s21_consume_f(ctx, &st->wleft);
        }
        if (s21_can_read_f(ctx, st->wleft) && s21_peek_f(ctx) == (unsigned char)')') {
          s21_consume_f(ctx, &st->wleft);
        }
      }

      ok = 1;
    } else if (s21_match_ci_kw(ctx->in, st->wleft, "infinity", &klen)) {
      int n = klen;
      *out = INFINITY;
      while (n > 0) {
        s21_consume_f(ctx, &st->wleft);
        n--;
      }
      ok = 1;
    } else if (s21_match_ci_kw(ctx->in, st->wleft, "inf", &klen)) {
      int n = klen;
      *out = INFINITY;
      while (n > 0) {
        s21_consume_f(ctx, &st->wleft);
        n--;
      }
      ok = 1;
    }
  }

  return ok;
}
