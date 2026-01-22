#include "s21_sscanf_internal.h"

/*
файл отвечает за разбор строки формата sscanf — превращает %... в структуру
токена (подавление *, ширина, модификатор длины, спецификатор и scanset %[...])
*/

// Разбирает символ подавления присваивания '*' после '%'.
static const char *s21_parse_suppress(const char *p, s21_fmt_token_t *t);

// Разбирает ширину поля (последовательность цифр) после '*' и до модификатора
// длины/спецификатора.
static const char *s21_parse_width(const char *p, s21_fmt_token_t *t);

// Разбирает модификатор длины: hh/h/l/ll/L.
static const char *s21_parse_length(const char *p, s21_fmt_token_t *t);

// Строит таблицу scanset для %[...], двигает указатель формата за закрывающую
// ']' и выставляет инверсию '^'.
static int s21_fill_scanset(unsigned char set[S21_SSCANF_SET_SIZE],
                            const char **pp, int *invert);

// Инициализирует токен формата значениями по умолчанию и обнуляет таблицу
// scanset.
void s21_init_token(s21_fmt_token_t *t) {
  int i = 0;

  if (t != NULL) {
    t->suppress = 0;
    t->width = 0;
    t->len = S21_LEN_NONE;
    t->spec = '\0';
    t->scanset_used = 0;
    t->scanset_invert = 0;

    while (i < S21_SSCANF_SET_SIZE) {
      t->set[i] = 0U;
      i++;
    }
  }
}

// Разбирает одну спецификацию преобразования, начинающуюся с '%' (флаги,
// ширина, длина, тип, включая %[...]). Возвращает указатель на следующий символ
// формата после спецификатора, либо fmt при ошибке парсинга.
const char *s21_parse_token(const char *fmt, s21_fmt_token_t *t) {
  const char *res = fmt;
  const char *p = fmt;
  int ok = 0;

  if (fmt != NULL && t != NULL && *fmt == '%') {
    ok = 1;
    s21_init_token(t);

    p = fmt + 1;
    if (*p == '\0') {
      ok = 0;
    }

    if (ok) p = s21_parse_suppress(p, t);
    if (ok) p = s21_parse_width(p, t);
    if (ok) p = s21_parse_length(p, t);

    if (ok) {
      if (*p == '\0') {
        ok = 0;
      } else if (*p == '[') {
        t->spec = '[';
        t->scanset_used = 1;
        if (!s21_fill_scanset(t->set, &p, &t->scanset_invert)) {
          ok = 0;
        }
      } else {
        t->spec = *p;
        p++;
      }
    }

    if (ok) {
      res = p;
    } else {
      t->spec = '\0';
      res = fmt;
    }
  } else if (t != NULL) {
    s21_init_token(t);
  }

  return res;
}

// Разбирает '*' (подавление присваивания) и сдвигает указатель формата дальше.
static const char *s21_parse_suppress(const char *p, s21_fmt_token_t *t) {
  const char *q = p;

  if (q != NULL && t != NULL) {
    if (*q == '*') {
      t->suppress = 1;
      q++;
    }
  }

  return q;
}

// Разбирает ширину поля (целое число) и записывает её в токен; при переполнении
// насыщает до INT_MAX.
static const char *s21_parse_width(const char *p, s21_fmt_token_t *t) {
  const char *q = p;
  int width = 0;
  int has_digits = 0;

  if (q != NULL && t != NULL) {
    while (*q != '\0' && s21_is_digit((unsigned char)*q)) {
      int digit = (int)((unsigned char)*q - (unsigned char)'0');
      has_digits = 1;

      if (width <= (INT_MAX - digit) / 10) {
        width = width * 10 + digit;
      } else {
        width = INT_MAX;
      }

      q++;
    }

    if (has_digits) {
      t->width = width;
    }
  }

  return q;
}

// Разбирает модификатор длины (hh/h/l/ll/L) и сдвигает указатель формата
// дальше.
static const char *s21_parse_length(const char *p, s21_fmt_token_t *t) {
  const char *q = p;

  if (q != NULL && t != NULL && *q != '\0') {
    if (*q == 'h') {
      if (*(q + 1) == 'h') {
        t->len = S21_LEN_HH;
        q += 2;
      } else {
        t->len = S21_LEN_H;
        q += 1;
      }
    } else if (*q == 'l') {
      if (*(q + 1) == 'l') {
        t->len = S21_LEN_LL;
        q += 2;
      } else {
        t->len = S21_LEN_L;
        q += 1;
      }
    } else if (*q == 'L') {
      t->len = S21_LEN_CAP_L;
      q += 1;
    }
  }

  return q;
}

// Проверяет, является ли текущий символ '-' частью диапазона в scanset
// (например, a-z), а не литералом '-'.
static int s21_is_range_dash(const char *p, int first_in_list) {
  int ok = 0;

  if (p != NULL) {
    if (!first_in_list && *p == '-' && *(p + 1) != '\0' && *(p + 1) != ']') {
      ok = 1;
    }
  }

  return ok;
}

// Отмечает один символ как входящий в scanset.
static void s21_set_byte(unsigned char set[S21_SSCANF_SET_SIZE],
                         unsigned char c) {
  set[(int)c] = 1U;
}

// Отмечает диапазон символов [a..b] как входящий в scanset (если a > b,
// диапазон разворачивается).
static void s21_set_range(unsigned char set[S21_SSCANF_SET_SIZE],
                          unsigned char a, unsigned char b) {
  unsigned int lo = (unsigned int)a;
  unsigned int hi = (unsigned int)b;

  if (lo > hi) {
    unsigned int tmp = lo;
    lo = hi;
    hi = tmp;
  }

  while (lo <= hi) {
    set[lo] = 1U;
    lo++;
  }
}

/*
Заполняет таблицу scanset и сдвигает *pp на символ после закрывающей ']'.
Вход: *pp указывает на '['
Выход:
  set заполнен отметками символов
  *invert = 1, если указан '^' (инверсия)
  *pp указывает на символ после ']'

Возвращает: 1 при успехе, 0 если закрывающая ']' не найдена (ошибка парсинга)
*/
static int s21_fill_scanset(unsigned char set[S21_SSCANF_SET_SIZE],
                            const char **pp, int *invert) {
  const char *p = (pp != NULL) ? *pp : NULL;
  int ok = 0;
  int i = 0;

  if (set != NULL && p != NULL && invert != NULL && *p == '[') {
    while (i < S21_SSCANF_SET_SIZE) {
      set[i] = 0U;
      i++;
    }

    p++;  // после '['
    *invert = 0;

    if (*p == '^') {
      *invert = 1;
      p++;
    }

    // Если ']' стоит первым в списке, то это обычный символ набора
    if (*p == ']') {
      s21_set_byte(set, (unsigned char)']');
      p++;
    }

    // Читаем элементы набора до закрывающей ']'
    {
      unsigned char prev = 0U;
      int have_prev = 0;
      int first = 1;

      while (*p != '\0' && *p != ']') {
        unsigned char cur = (unsigned char)*p;

        if (have_prev && s21_is_range_dash(p, first) && *(p + 1) != '\0') {
          unsigned char nextc = (unsigned char)*(p + 1);
          s21_set_range(set, prev, nextc);
          p += 2;
          have_prev = 0;
        } else {
          s21_set_byte(set, cur);
          prev = cur;
          have_prev = 1;
          p++;
        }

        first = 0;
      }
    }

    if (*p == ']') {
      p++;      // потребляем закрывающую ']'
      *pp = p;  // теперь указывает на символ после ']'
      ok = 1;
    }
  }

  return ok;
}
