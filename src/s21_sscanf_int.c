#include "s21_sscanf_internal.h"

static int s21_digit_val(unsigned char c);
static int s21_is_digit_base(unsigned char c, int base);
static int s21_effective_width(int width);
static void s21_restore_ctx(s21_scan_ctx_t *ctx, const char *in0, int consumed0);
static int s21_can_read(const s21_scan_ctx_t *ctx, int wleft);
static int s21_peek_char(const s21_scan_ctx_t *ctx);
static void s21_accumulate_ull(unsigned long long *acc, int digit, int base);
static void s21_read_optional_sign(s21_scan_ctx_t *ctx, int *wleft, int allow_sign, int *sign);
static int s21_detect_base_auto(const s21_scan_ctx_t *ctx, int wleft);
static void s21_consume_hex_prefix_if_valid(s21_scan_ctx_t *ctx, int *wleft, int base);

/* 
файл отвечает за разбор целочисленных и указательных преобразований sscanf 
(ядро чтения числа с выбором базы/знаком/шириной и обёртки для %d/%i/%u/%o/%x/%X/%p 
с последующей записью результата)
*/

// Ядро разбора целого числа: читает знак (опционально), определяет основание (в т.ч. AUTO для %i),
// затем потребляет цифры с учётом ширины и накапливает модуль в unsigned long long.
s21_scan_status_t s21_read_int_core(s21_scan_ctx_t *ctx, int width,
                                    s21_base_mode_t base_mode, int allow_sign,
                                    unsigned long long *mag, int *sign) {
  s21_scan_status_t res = S21_SCAN_MATCH_FAIL;

  if (ctx == NULL || ctx->in == NULL || mag == NULL || sign == NULL) {
    res = S21_SCAN_MATCH_FAIL;
  } else {
    const char *in0 = ctx->in;
    int consumed0 = ctx->consumed;

    int wleft = s21_effective_width(width);
    int base = (base_mode == S21_BASE_AUTO) ? 10 : (int)base_mode;
    unsigned long long acc = 0ULL;
    int digits = 0;

    *mag = 0ULL;
    *sign = 1;

    if (!s21_can_read(ctx, wleft)) {
      ctx->input_fail = 1;
      res = S21_SCAN_INPUT_FAIL;
    } else {
      s21_read_optional_sign(ctx, &wleft, allow_sign, sign);
      base = (base_mode == S21_BASE_AUTO) ? s21_detect_base_auto(ctx, wleft) : base;
      s21_consume_hex_prefix_if_valid(ctx, &wleft, base);

      while (s21_can_read(ctx, wleft) &&
             s21_is_digit_base((unsigned char)s21_peek_char(ctx), base)) {
        unsigned char ch = 0U;
        int dv = s21_digit_val((unsigned char)s21_peek_char(ctx));
        (void)s21_take_in(ctx, &ch);
        s21_accumulate_ull(&acc, dv, base);
        wleft--;
        digits++;
      }

      if (digits > 0) {
        *mag = acc;
        res = S21_SCAN_OK;
      } else {
        res = S21_SCAN_MATCH_FAIL;
        s21_restore_ctx(ctx, in0, consumed0);
        if (*in0 == '\0') {
          ctx->input_fail = 1;
          res = S21_SCAN_INPUT_FAIL;
        }
      }
    }
  }

  return res;
}

// Обрабатывает знаковые целые спецификаторы (например %d и %i): вызывает ядро разбора, применяет знак и насыщение,
// затем при необходимости пишет результат в аргумент пользователя с учётом модификатора длины.
s21_scan_status_t s21_scan_signed(s21_scan_ctx_t *ctx, const s21_fmt_token_t *t,
                                  s21_base_mode_t base_mode) {
  s21_scan_status_t res = S21_SCAN_MATCH_FAIL;

  if (ctx == NULL || t == NULL) {
    res = S21_SCAN_MATCH_FAIL;
  } else {
    unsigned long long mag = 0ULL;
    int sgn = 1;
    long long outv = 0LL;

    res = s21_read_int_core(ctx, t->width, base_mode, 1, &mag, &sgn);
    if (res == S21_SCAN_OK) {
      if (sgn < 0) {
        unsigned long long lim = (unsigned long long)LLONG_MAX + 1ULL;
        outv = (mag >= lim) ? LLONG_MIN : -(long long)mag;
      } else {
        outv = (mag > (unsigned long long)LLONG_MAX) ? LLONG_MAX : (long long)mag;
      }

      if (!t->suppress && ctx->ap != NULL) {
        s21_store_signed(ctx->ap, t->len, outv);
      }
    }
  }

  return res;
}

// Обрабатывает беззнаковые целые спецификаторы (например %u/%o/%x/%X): читает модуль (и возможный знак),
// затем при необходимости пишет результат в аргумент пользователя с учётом модификатора длины.
s21_scan_status_t s21_scan_unsigned(s21_scan_ctx_t *ctx, const s21_fmt_token_t *t,
                                    int base) {
  s21_scan_status_t res = S21_SCAN_MATCH_FAIL;

  if (ctx == NULL || t == NULL) {
    res = S21_SCAN_MATCH_FAIL;
  } else {
    unsigned long long mag = 0ULL;
    int sgn = 1;
    unsigned long long outv = 0ULL;

    res = s21_read_int_core(ctx, t->width, (s21_base_mode_t)base, 1, &mag, &sgn);
    if (res == S21_SCAN_OK) {
      outv = (sgn < 0) ? (0ULL - mag) : mag;
      if (!t->suppress && ctx->ap != NULL) {
        s21_store_unsigned(ctx->ap, t->len, outv);
      }
    }
  }

  return res;
}

// Обрабатывает %p: читает шестнадцатеричное значение (опционально с 0x при наличии валидных цифр),
// затем при необходимости пишет указатель в аргумент пользователя.
s21_scan_status_t s21_scan_pointer(s21_scan_ctx_t *ctx, const s21_fmt_token_t *t) {
  s21_scan_status_t res = S21_SCAN_MATCH_FAIL;

  if (ctx == NULL || t == NULL) {
    res = S21_SCAN_MATCH_FAIL;
  } else {
    unsigned long long mag = 0ULL;
    int sgn = 1;
    void *outp = NULL;

    res = s21_read_int_core(ctx, t->width, S21_BASE_16, 0, &mag, &sgn);
    if (res == S21_SCAN_OK) {
      outp = (void *)(uintptr_t)mag;
      if (!t->suppress && ctx->ap != NULL) {
        s21_store_pointer(ctx->ap, outp);
      }
    }
  }

  return res;
}


// Преобразует один символ в значение цифры: 0..15 для 0-9/a-f/A-F, либо -1 если символ не цифра.
static int s21_digit_val(unsigned char c) {
  int v = -1;

  if (c >= (unsigned char)'0' && c <= (unsigned char)'9') {
    v = (int)(c - (unsigned char)'0');
  } else if (c >= (unsigned char)'a' && c <= (unsigned char)'f') {
    v = 10 + (int)(c - (unsigned char)'a');
  } else if (c >= (unsigned char)'A' && c <= (unsigned char)'F') {
    v = 10 + (int)(c - (unsigned char)'A');
  }

  return v;
}

// Проверяет, допустим ли символ как цифра в заданной базе (8/10/16).
static int s21_is_digit_base(unsigned char c, int base) {
  int ok = 0;
  int v = s21_digit_val(c);

  if (v >= 0 && v < base) ok = 1;
  return ok;
}

// Возвращает эффективную ширину: если width не задана (<=0), то считаем её бесконечной (INT_MAX).
static int s21_effective_width(int width) {
  int w = width;
  if (w <= 0) w = INT_MAX;
  return w;
}

// Откатывает контекст ввода на сохранённые значения (нужно при mismatch, когда ничего не прочитали).
static void s21_restore_ctx(s21_scan_ctx_t *ctx, const char *in0, int consumed0) {
  if (ctx != NULL) {
    ctx->in = in0;
    ctx->consumed = consumed0;
  }
}

// Проверяет, можно ли читать следующий символ: есть ширина, не конец строки и валидный ctx.
static int s21_can_read(const s21_scan_ctx_t *ctx, int wleft) {
  int ok = 0;
  if (ctx != NULL && ctx->in != NULL && wleft > 0 && *ctx->in != '\0') ok = 1;
  return ok;
}

// Возвращает текущий символ входа как int без потребления (0, если ctx некорректен).
static int s21_peek_char(const s21_scan_ctx_t *ctx) {
  int c = 0;
  if (ctx != NULL && ctx->in != NULL) c = (unsigned char)(*ctx->in);
  return c;
}

// Накапливает очередную цифру в acc в заданной базе; при переполнении насыщает до ULLONG_MAX.
static void s21_accumulate_ull(unsigned long long *acc, int digit, int base) {
  if (acc != NULL) {
    unsigned long long b = (unsigned long long)base;
    unsigned long long d = (unsigned long long)digit;

    if (*acc > (ULLONG_MAX - d) / b) {
      *acc = ULLONG_MAX;
    } else {
      *acc = (*acc) * b + d;
    }
  }
}

// Читает необязательный знак '+'/'-' (если разрешено allow_sign) и обновляет sign и оставшуюся ширину.
static void s21_read_optional_sign(s21_scan_ctx_t *ctx, int *wleft, int allow_sign, int *sign) {
  if (ctx != NULL && wleft != NULL && sign != NULL && allow_sign && s21_can_read(ctx, *wleft)) {
    unsigned char c = (unsigned char)s21_peek_char(ctx);
    if (c == (unsigned char)'+' || c == (unsigned char)'-') {
      unsigned char tmp = 0U;
      if (c == (unsigned char)'-') *sign = -1;
      (void)s21_take_in(ctx, &tmp);
      (*wleft)--;
    }
  }
}

// В режиме AUTO (%i) выбирает базу: по ведущему '0' => 8, по "0x" + валидной hex-цифре => 16, иначе 10.
static int s21_detect_base_auto(const s21_scan_ctx_t *ctx, int wleft) {
  int base = 10;

  if (ctx != NULL && ctx->in != NULL && wleft > 0 && (unsigned char)ctx->in[0] == (unsigned char)'0') {
    base = 8;
    if (wleft >= 3) {
      unsigned char c1 = (unsigned char)ctx->in[1];
      unsigned char c2 = (unsigned char)ctx->in[2];
      if ((c1 == (unsigned char)'x' || c1 == (unsigned char)'X') && s21_is_xdigit(c2)) {
        base = 16;
      }
    }
  }

  return base;
}

// Для базы 16 потребляет префикс "0x"/"0X", но только если после него реально идёт hex-цифра (и хватает ширины).
static void s21_consume_hex_prefix_if_valid(s21_scan_ctx_t *ctx, int *wleft, int base) {
  if (ctx != NULL && wleft != NULL && base == 16 && ctx->in != NULL && *wleft >= 3) {
    unsigned char c0 = (unsigned char)ctx->in[0];
    unsigned char c1 = (unsigned char)ctx->in[1];
    unsigned char c2 = (unsigned char)ctx->in[2];

    if (c0 == (unsigned char)'0' && (c1 == (unsigned char)'x' || c1 == (unsigned char)'X') &&
        s21_is_xdigit(c2)) {
      unsigned char tmp = 0U;
      (void)s21_take_in(ctx, &tmp);
      (void)s21_take_in(ctx, &tmp);
      *wleft -= 2;
    }
  }
}
