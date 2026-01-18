#include "s21_sscanf.h"
#include "s21_sscanf_internal.h"

/* ---------- internal helpers ---------- */

static const char *s21_skip_spaces_fmt(const char *p) {
  const char *q = p;
  int cont = 1;

  while (cont) {
    if (q != NULL && *q != '\0' && s21_is_space((unsigned char)*q)) {
      q++;
    } else {
      cont = 0;
    }
  }

  return q;
}

static s21_scan_status_t s21_match_literal(s21_scan_ctx_t *ctx, unsigned char need) {
  s21_scan_status_t st = S21_SCAN_MATCH_FAIL;

  if (ctx == NULL || ctx->in == NULL) {
    st = S21_SCAN_MATCH_FAIL;
  } else if (*ctx->in == '\0') {
    ctx->input_fail = 1;
    st = S21_SCAN_INPUT_FAIL;
  } else if ((unsigned char)*ctx->in == need) {
    unsigned char tmp = 0U;
    (void)s21_take_in(ctx, &tmp);
    st = S21_SCAN_OK;
  } else {
    st = S21_SCAN_MATCH_FAIL;
  }

  return st;
}

static int s21_need_skip_input_spaces(char spec) {
  int skip = 0;

  if (spec != 'c' && spec != '[' && spec != 'n' && spec != '%') {
    skip = 1;
  }

  return skip;
}

static void s21_stop_on_status(s21_scan_ctx_t *ctx, s21_scan_status_t st) {
  if (ctx != NULL) {
    if (st == S21_SCAN_INPUT_FAIL) {
      ctx->input_fail = 1;
      ctx->stopped = 1;
    } else if (st == S21_SCAN_MATCH_FAIL) {
      ctx->match_fail = 1;
      ctx->stopped = 1;
    }
  }
}

/* ---------- % and %n conversions (kept here as "small special cases") ---------- */

s21_scan_status_t s21_scan_percent(s21_scan_ctx_t *ctx, const s21_fmt_token_t *t) {
  s21_scan_status_t st = S21_SCAN_MATCH_FAIL;

  if (ctx == NULL || t == NULL) {
    st = S21_SCAN_MATCH_FAIL;
  } else {
    st = s21_match_literal(ctx, (unsigned char)'%');
  }

  return st;
}

s21_scan_status_t s21_scan_n(s21_scan_ctx_t *ctx, const s21_fmt_token_t *t) {
  s21_scan_status_t st = S21_SCAN_MATCH_FAIL;

  if (ctx == NULL || t == NULL) {
    st = S21_SCAN_MATCH_FAIL;
  } else {
    if (!t->suppress && ctx->ap != NULL) {
      s21_store_n(ctx->ap, t->len, ctx->consumed);
    }
    st = S21_SCAN_OK;
  }

  return st;
}

/* ---------- dispatcher ---------- */

void s21_apply_token(s21_scan_ctx_t *ctx, const s21_fmt_token_t *t) {
  s21_scan_status_t st = S21_SCAN_MATCH_FAIL;
  int do_assign_count = 0;

  if (ctx == NULL || t == NULL) {
    st = S21_SCAN_MATCH_FAIL;
  } else {
    if (s21_need_skip_input_spaces(t->spec)) s21_skip_spaces_in(ctx);

    if (t->spec == 'd') {
      st = s21_scan_signed(ctx, t, S21_BASE_10);
      do_assign_count = (!t->suppress);
    } else if (t->spec == 'i') {
      st = s21_scan_signed(ctx, t, S21_BASE_AUTO);
      do_assign_count = (!t->suppress);
    } else if (t->spec == 'u') {
      st = s21_scan_unsigned(ctx, t, 10);
      do_assign_count = (!t->suppress);
    } else if (t->spec == 'o') {
      st = s21_scan_unsigned(ctx, t, 8);
      do_assign_count = (!t->suppress);
    } else if (t->spec == 'x' || t->spec == 'X') {
      st = s21_scan_unsigned(ctx, t, 16);
      do_assign_count = (!t->suppress);
    } else if (t->spec == 'p') {
      st = s21_scan_pointer(ctx, t);
      do_assign_count = (!t->suppress);
    } else if (t->spec == 'f' || t->spec == 'e' || t->spec == 'E' ||
               t->spec == 'g' || t->spec == 'G') {
      st = s21_scan_float(ctx, t);
      do_assign_count = (!t->suppress);
    } else if (t->spec == 's') {
      st = s21_scan_string(ctx, t);
      do_assign_count = (!t->suppress);
    } else if (t->spec == 'c') {
      st = s21_scan_char(ctx, t);
      do_assign_count = (!t->suppress);
    } else if (t->spec == '[') {
      st = s21_scan_scanset(ctx, t);
      do_assign_count = (!t->suppress);
    } else if (t->spec == 'n') {
      st = s21_scan_n(ctx, t);
      /* macOS sscanf(): %n does not contribute to return value */
      do_assign_count = 0;
    } else if (t->spec == '%') {
      st = s21_scan_percent(ctx, t);
      do_assign_count = 0;
    } else {
      st = S21_SCAN_MATCH_FAIL;
    }

    if (st == S21_SCAN_OK && do_assign_count) ctx->assigned++;
    s21_stop_on_status(ctx, st);
  }
}

/* ---------- public API ---------- */

int s21_sscanf(const char *str, const char *format, ...) {
  int ret = 0;

  if (str == NULL || format == NULL) {
    ret = 0;
  } else {
    s21_scan_ctx_t ctx;
    s21_fmt_token_t tok;

    va_list ap;
    va_start(ap, format);

    ctx.in = str;
    ctx.consumed = 0;
    ctx.assigned = 0;
    ctx.stopped = 0;
    ctx.input_fail = 0;
    ctx.match_fail = 0;
    ctx.ap = &ap;

    {
      const char *p = format;
      while (*p != '\0' && !ctx.stopped) {
        if (s21_is_space((unsigned char)*p)) {
          p = s21_skip_spaces_fmt(p);
          s21_skip_spaces_in(&ctx);
        } else if (*p != '%') {
          s21_scan_status_t st = s21_match_literal(&ctx, (unsigned char)*p);
          p++;
          s21_stop_on_status(&ctx, st);
        } else {
          const char *next = s21_parse_token(p, &tok);
          if (tok.spec == '\0' || next == p) {
            ctx.match_fail = 1;
            ctx.stopped = 1;
          } else {
            p = next;
            s21_apply_token(&ctx, &tok);
          }
        }
      }
    }

    va_end(ap);

    if (ctx.assigned == 0 && ctx.input_fail) {
      ret = -1; /* EOF */
    } else {
      ret = ctx.assigned;
    }
  }

  return ret;
}
