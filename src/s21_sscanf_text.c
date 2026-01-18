#include "s21_sscanf_internal.h"

/*
отвечает за разбор и чтение текстовых спецификаторов sscanf - %s, %c и %[...] (scanset)
*/

static int s21_effective_width_text(int width, int defv);
static int s21_scanset_accept(const s21_fmt_token_t *t, unsigned char c);
static void s21_write_byte(char *dst, int do_write, int idx, unsigned char c);

// Реализует спецификатор %s: пропускает пробелы, читает непробельную последовательность с учётом width и пишет в буфер (если не подавлено '*').
s21_scan_status_t s21_scan_string(s21_scan_ctx_t *ctx, const s21_fmt_token_t *t) {
  s21_scan_status_t res = S21_SCAN_MATCH_FAIL;

  if (ctx == NULL || t == NULL || ctx->in == NULL) {
    res = S21_SCAN_MATCH_FAIL;
  } else {
    int limit = s21_effective_width_text(t->width, INT_MAX);
    int nread = 0;
    int do_write = 0;
    char *dst = NULL;

    s21_skip_spaces_in(ctx);

    if (*ctx->in == '\0') {
      ctx->input_fail = 1;
      res = S21_SCAN_INPUT_FAIL;
    } else {
      if (!t->suppress && ctx->ap != NULL) {
        dst = va_arg(*ctx->ap, char *);
        do_write = (dst != NULL);
      }

      while (*ctx->in != '\0' && nread < limit &&
             !s21_is_space((unsigned char)*ctx->in)) {
        unsigned char ch = 0U;
        (void)s21_take_in(ctx, &ch);
        s21_write_byte(dst, do_write, nread, ch);
        nread++;
      }

      if (nread > 0) {
        if (do_write) dst[nread] = '\0';
        res = S21_SCAN_OK;
      } else {
        res = S21_SCAN_MATCH_FAIL;
      }
    }
  }

  return res;
}

// Реализует спецификатор %c: читает ровно width символов (по умолчанию 1), пробелы не пропускает; при успехе пишет байты в буфер
s21_scan_status_t s21_scan_char(s21_scan_ctx_t *ctx, const s21_fmt_token_t *t) {
  s21_scan_status_t res = S21_SCAN_MATCH_FAIL;

  if (ctx == NULL || t == NULL || ctx->in == NULL) {
    res = S21_SCAN_MATCH_FAIL;
  } else {
    int limit = s21_effective_width_text(t->width, 1);
    int nread = 0;
    int do_write = 0;
    char *dst = NULL;

    if (!t->suppress && ctx->ap != NULL) {
      dst = va_arg(*ctx->ap, char *);
      do_write = (dst != NULL);
    }

    while (nread < limit && *ctx->in != '\0') {
      unsigned char ch = 0U;
      (void)s21_take_in(ctx, &ch);
      if (do_write) dst[nread] = (char)ch;
      nread++;
    }

    if (nread == limit) {
      res = S21_SCAN_OK;
    } else {
      ctx->input_fail = 1;
      res = S21_SCAN_INPUT_FAIL;
    }
  }

  return res;
}

// Реализует спецификатор %[...]: читает символы, входящие (или не входящие при '^') в набор, с учётом width; при успехе пишет строку и завершающий '\0'.
s21_scan_status_t s21_scan_scanset(s21_scan_ctx_t *ctx, const s21_fmt_token_t *t) {
  s21_scan_status_t res = S21_SCAN_MATCH_FAIL;

  if (ctx == NULL || t == NULL || ctx->in == NULL || !t->scanset_used) {
    res = S21_SCAN_MATCH_FAIL;
  } else {
    int limit = s21_effective_width_text(t->width, INT_MAX);
    int nread = 0;
    int do_write = 0;
    char *dst = NULL;

    if (*ctx->in == '\0') {
      ctx->input_fail = 1;
      res = S21_SCAN_INPUT_FAIL;
    } else {
      if (!t->suppress && ctx->ap != NULL) {
        dst = va_arg(*ctx->ap, char *);
        do_write = (dst != NULL);
      }

      while (*ctx->in != '\0' && nread < limit &&
             s21_scanset_accept(t, (unsigned char)*ctx->in)) {
        unsigned char ch = 0U;
        (void)s21_take_in(ctx, &ch);
        s21_write_byte(dst, do_write, nread, ch);
        nread++;
      }

      if (nread > 0) {
        if (do_write) dst[nread] = '\0';
        res = S21_SCAN_OK;
      } else {
        res = S21_SCAN_MATCH_FAIL;
      }
    }
  }

  return res;
}

/* вспомогательные */

// Возвращает эффективную ширину для текстовых спецификаторов: если width не задана, подставляет defv.
static int s21_effective_width_text(int width, int defv) {
  int w = width;
  if (w <= 0) w = defv;
  return w;
}

// Проверяет, принимает ли scanset символ "c": учитывает таблицу t->set и инверсию '^'.
static int s21_scanset_accept(const s21_fmt_token_t *t, unsigned char c) {
  int ok = 0;

  if (t != NULL) {
    int in_set = (t->set[(int)c] != 0U);
    ok = t->scanset_invert ? !in_set : in_set;
  }

  return ok;
}

// Пишет один байт в буфер по индексу idx, если запись разрешена (не подавлено '*' и dst не NULL).
static void s21_write_byte(char *dst, int do_write, int idx, unsigned char c) {
  if (do_write && dst != NULL && idx >= 0) {
    dst[idx] = (char)c;
  }
}
