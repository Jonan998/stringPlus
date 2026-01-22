#include "s21_sscanf_internal.h"

/* базовые примитивы “чтения потока” и ASCII-проверки */

// Проверяет, является ли символ пробельным (по ASCII): пробел, таб, перевод
// строки и т.п.
int s21_is_space(unsigned char c) {
  int res = 0;
  if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' ||
      c == '\f') {
    res = 1;
  }
  return res;
}

// Проверяет, является ли символ десятичной цифрой '0'..'9'.
int s21_is_digit(unsigned char c) {
  int res = 0;
  if (c >= '0' && c <= '9') {
    res = 1;
  }
  return res;
}

// Проверяет, является ли символ шестнадцатеричной цифрой: 0-9, a-f, A-F.
int s21_is_xdigit(unsigned char c) {
  int res = 0;
  if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') ||
      (c >= 'A' && c <= 'F')) {
    res = 1;
  }
  return res;
}

// Приводит ASCII-букву к нижнему регистру; остальные символы возвращает без
// изменений.
unsigned char s21_tolower_ascii(unsigned char c) {
  unsigned char res = c;
  if (c >= 'A' && c <= 'Z') {
    res = (unsigned char)(c - 'A' + 'a');
  }
  return res;
}

// Смотрит текущий байт входа без потребления: записывает его в out и возвращает
// 1; если конец строки — 0.
int s21_peek_in(s21_scan_ctx_t *ctx, unsigned char *out) {
  int ok = 0;

  if (ctx != NULL && ctx->in != NULL && out != NULL) {
    if (*ctx->in != '\0') {
      *out = (unsigned char)*ctx->in;
      ok = 1;
    }
  }

  return ok;
}

// Потребляет один байт входа: записывает его в out, сдвигает курсор и
// увеличивает счётчик consumed; на '\0' ставит input_fail.
int s21_take_in(s21_scan_ctx_t *ctx, unsigned char *out) {
  int ok = 0;

  if (ctx != NULL && ctx->in != NULL && out != NULL) {
    if (*ctx->in != '\0') {
      *out = (unsigned char)*ctx->in;
      ctx->in++;
      ctx->consumed++;
      ok = 1;
    } else {
      ctx->input_fail = 1;
    }
  }

  return ok;
}

// Пропускает (потребляет) все пробельные символы во входе, пока они встречаются
// подряд.
void s21_skip_spaces_in(s21_scan_ctx_t *ctx) {
  unsigned char c = 0;
  int cont = 1;

  if (ctx != NULL && ctx->in != NULL) {
    while (cont) {
      if (s21_peek_in(ctx, &c) && s21_is_space(c)) {
        (void)s21_take_in(ctx, &c);
      } else {
        cont = 0;
      }
    }
  }
}

// Потребляет символы, пока pred(c) истинно и не превышен лимит limit;
// возвращает количество потреблённых байтов.
int s21_take_while(s21_scan_ctx_t *ctx, int limit, int (*pred)(unsigned char)) {
  int taken = 0;
  int maxn = limit;
  unsigned char c = 0;
  int cont = 1;

  if (maxn <= 0) {
    maxn = INT_MAX;
  }

  if (ctx != NULL && ctx->in != NULL && pred != NULL) {
    while (cont) {
      if (taken < maxn && s21_peek_in(ctx, &c) && pred(c)) {
        (void)s21_take_in(ctx, &c);
        taken++;
      } else {
        cont = 0;
      }
    }
  }

  return taken;
}
