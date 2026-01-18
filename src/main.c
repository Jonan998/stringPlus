#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <math.h>

#include "s21_sscanf.h"

/* вспомогательные */

static int s21t_strlen(const char *s) {
  int n = 0;
  if (s != NULL) {
    while (s[n] != '\0') n++;
  }
  return n;
}

static int s21t_streq(const char *a, const char *b) {
  int eq = 1;
  int i = 0;
  if (a == NULL || b == NULL) {
    eq = (a == b);
  } else {
    while (eq && (a[i] != '\0' || b[i] != '\0')) {
      if (a[i] != b[i]) eq = 0;
      i++;
    }
  }
  return eq;
}

static int s21t_memeq(const unsigned char *a, const unsigned char *b, int n) {
  int eq = 1;
  int i = 0;
  if (a == NULL || b == NULL) {
    eq = (a == b);
  } else {
    while (eq && i < n) {
      if (a[i] != b[i]) eq = 0;
      i++;
    }
  }
  return eq;
}

static int s21t_iabs(int x) {
  return (x < 0) ? -x : x;
}

static int s21t_float_eq(double a, double b) {
  int ok = 0;
  double da = a;
  double db = b;

  if (isnan(da) && isnan(db)) ok = 1;
  else if (isinf(da) && isinf(db) && ((da > 0) == (db > 0))) ok = 1;
  else {
    double diff = fabs(da - db);
    double scale = fmax(1.0, fmax(fabs(da), fabs(db)));
    ok = (diff <= 1e-9 * scale);
  }
  return ok;
}

/* тест */

typedef struct {
  int total;
  int passed;
} s21t_stats;

static void s21t_fail_header(const char *name, const char *in, const char *fmt) {
  printf("\n[FAIL] %s\n  in : \"%s\"\n  fmt: \"%s\"\n", name, in, fmt);
}

static void s21t_ok(s21t_stats *st) {
  if (st != NULL) {
    st->total++;
    st->passed++;
  }
}

static void s21t_fail(s21t_stats *st) {
  if (st != NULL) {
    st->total++;
  }
}

/* паттерны для обычных типов */

static void test_int(s21t_stats *st, const char *name, const char *in, const char *fmt) {
  int r_std = 0, r_s21 = 0;
  int a_std = 777, a_s21 = 777;

  r_std = sscanf(in, fmt, &a_std);
  r_s21 = s21_sscanf(in, fmt, &a_s21);

  if (r_std == r_s21 && a_std == a_s21) {
    s21t_ok(st);
  } else {
    s21t_fail(st);
    s21t_fail_header(name, in, fmt);
    printf("  std: r=%d a=%d\n", r_std, a_std);
    printf("  s21: r=%d a=%d\n", r_s21, a_s21);
  }
}

static void test_ll(s21t_stats *st, const char *name, const char *in, const char *fmt) {
  int r_std = 0, r_s21 = 0;
  long long a_std = 0, a_s21 = 0;

  a_std = 0x1122334455667788LL;
  a_s21 = 0x1122334455667788LL;

  r_std = sscanf(in, fmt, &a_std);
  r_s21 = s21_sscanf(in, fmt, &a_s21);

  if (r_std == r_s21 && a_std == a_s21) s21t_ok(st);
  else {
    s21t_fail(st);
    s21t_fail_header(name, in, fmt);
    printf("  std: r=%d a=%lld\n", r_std, a_std);
    printf("  s21: r=%d a=%lld\n", r_s21, a_s21);
  }
}

static void test_uint(s21t_stats *st, const char *name, const char *in, const char *fmt) {
  int r_std = 0, r_s21 = 0;
  unsigned int a_std = 0, a_s21 = 0;

  a_std = 0xDEADBEEFu;
  a_s21 = 0xDEADBEEFu;

  r_std = sscanf(in, fmt, &a_std);
  r_s21 = s21_sscanf(in, fmt, &a_s21);

  if (r_std == r_s21 && a_std == a_s21) s21t_ok(st);
  else {
    s21t_fail(st);
    s21t_fail_header(name, in, fmt);
    printf("  std: r=%d a=%u\n", r_std, a_std);
    printf("  s21: r=%d a=%u\n", r_s21, a_s21);
  }
}

static void test_ull(s21t_stats *st, const char *name, const char *in, const char *fmt) {
  int r_std = 0, r_s21 = 0;
  unsigned long long a_std = 0, a_s21 = 0;

  a_std = 0x1122334455667788ULL;
  a_s21 = 0x1122334455667788ULL;

  r_std = sscanf(in, fmt, &a_std);
  r_s21 = s21_sscanf(in, fmt, &a_s21);

  if (r_std == r_s21 && a_std == a_s21) s21t_ok(st);
  else {
    s21t_fail(st);
    s21t_fail_header(name, in, fmt);
    printf("  std: r=%d a=%llu\n", r_std, a_std);
    printf("  s21: r=%d a=%llu\n", r_s21, a_s21);
  }
}

static void test_short(s21t_stats *st, const char *name, const char *in, const char *fmt) {
  int r_std = 0, r_s21 = 0;
  short a_std = -1234, a_s21 = -1234;

  r_std = sscanf(in, fmt, &a_std);
  r_s21 = s21_sscanf(in, fmt, &a_s21);

  if (r_std == r_s21 && a_std == a_s21) s21t_ok(st);
  else {
    s21t_fail(st);
    s21t_fail_header(name, in, fmt);
    printf("  std: r=%d a=%hd\n", r_std, a_std);
    printf("  s21: r=%d a=%hd\n", r_s21, a_s21);
  }
}

static void test_char_len(s21t_stats *st, const char *name, const char *in, const char *fmt) {
  int r_std = 0, r_s21 = 0;
  signed char a_std = 0, a_s21 = 0;

  a_std = 55;
  a_s21 = 55;

  r_std = sscanf(in, fmt, &a_std);
  r_s21 = s21_sscanf(in, fmt, &a_s21);

  if (r_std == r_s21 && a_std == a_s21) s21t_ok(st);
  else {
    s21t_fail(st);
    s21t_fail_header(name, in, fmt);
    printf("  std: r=%d a=%d\n", r_std, (int)a_std);
    printf("  s21: r=%d a=%d\n", r_s21, (int)a_s21);
  }
}

static void test_str(s21t_stats *st, const char *name, const char *in, const char *fmt) {
  int r_std = 0, r_s21 = 0;
  char a_std[128];
  char a_s21[128];
  int i = 0;

  while (i < 128) {
    a_std[i] = (char)0;
    a_s21[i] = (char)0;
    i++;
  }

  r_std = sscanf(in, fmt, a_std);
  r_s21 = s21_sscanf(in, fmt, a_s21);

  if (r_std == r_s21 && s21t_streq(a_std, a_s21)) s21t_ok(st);
  else {
    s21t_fail(st);
    s21t_fail_header(name, in, fmt);
    printf("  std: r=%d a=\"%s\"\n", r_std, a_std);
    printf("  s21: r=%d a=\"%s\"\n", r_s21, a_s21);
  }
}

static void test_c1(s21t_stats *st, const char *name, const char *in, const char *fmt) {
  int r_std = 0, r_s21 = 0;
  char a_std = 0, a_s21 = 0;

  r_std = sscanf(in, fmt, &a_std);
  r_s21 = s21_sscanf(in, fmt, &a_s21);

  if (r_std == r_s21 && a_std == a_s21) s21t_ok(st);
  else {
    s21t_fail(st);
    s21t_fail_header(name, in, fmt);
    printf("  std: r=%d a=0x%02X\n", r_std, (unsigned char)a_std);
    printf("  s21: r=%d a=0x%02X\n", r_s21, (unsigned char)a_s21);
  }
}

static void test_cN(s21t_stats *st, const char *name, const char *in, const char *fmt, int n) {
  int r_std = 0, r_s21 = 0;
  unsigned char a_std[64];
  unsigned char a_s21[64];
  int i = 0;

  while (i < 64) {
    a_std[i] = 0xAA;
    a_s21[i] = 0xAA;
    i++;
  }

  r_std = sscanf(in, fmt, (char *)a_std);
  r_s21 = s21_sscanf(in, fmt, (char *)a_s21);

  if (r_std == r_s21 && s21t_memeq(a_std, a_s21, n)) s21t_ok(st);
  else {
    s21t_fail(st);
    s21t_fail_header(name, in, fmt);
    printf("  std: r=%d bytes:", r_std);
    for (i = 0; i < n; i++) printf(" %02X", a_std[i]);
    printf("\n  s21: r=%d bytes:", r_s21);
    for (i = 0; i < n; i++) printf(" %02X", a_s21[i]);
    printf("\n");
  }
}

static void test_float_d(s21t_stats *st, const char *name, const char *in, const char *fmt) {
  int r_std = 0, r_s21 = 0;
  double a_std = 0.0, a_s21 = 0.0;

  a_std = 123.456;
  a_s21 = 123.456;

  r_std = sscanf(in, fmt, &a_std);
  r_s21 = s21_sscanf(in, fmt, &a_s21);

  if (r_std == r_s21 && s21t_float_eq(a_std, a_s21)) s21t_ok(st);
  else {
    s21t_fail(st);
    s21t_fail_header(name, in, fmt);
    printf("  std: r=%d a=%.17g\n", r_std, a_std);
    printf("  s21: r=%d a=%.17g\n", r_s21, a_s21);
  }
}

static void test_float_f(s21t_stats *st, const char *name, const char *in, const char *fmt) {
  int r_std = 0, r_s21 = 0;
  float a_std = 0.0f, a_s21 = 0.0f;

  a_std = 12.5f;
  a_s21 = 12.5f;

  r_std = sscanf(in, fmt, &a_std);
  r_s21 = s21_sscanf(in, fmt, &a_s21);

  if (r_std == r_s21 && s21t_float_eq((double)a_std, (double)a_s21)) s21t_ok(st);
  else {
    s21t_fail(st);
    s21t_fail_header(name, in, fmt);
    printf("  std: r=%d a=%.9g\n", r_std, (double)a_std);
    printf("  s21: r=%d a=%.9g\n", r_s21, (double)a_s21);
  }
}

static void test_ptr(s21t_stats *st, const char *name, const char *in, const char *fmt) {
  int r_std = 0, r_s21 = 0;
  void *p_std = (void *)(uintptr_t)0x11111111u;
  void *p_s21 = (void *)(uintptr_t)0x11111111u;

  r_std = sscanf(in, fmt, &p_std);
  r_s21 = s21_sscanf(in, fmt, &p_s21);

  if (r_std == r_s21 && (uintptr_t)p_std == (uintptr_t)p_s21) s21t_ok(st);
  else {
    s21t_fail(st);
    s21t_fail_header(name, in, fmt);
    printf("  std: r=%d p=0x%llX\n", r_std, (unsigned long long)(uintptr_t)p_std);
    printf("  s21: r=%d p=0x%llX\n", r_s21, (unsigned long long)(uintptr_t)p_s21);
  }
}


static void test_n_then_int(s21t_stats *st, const char *name, const char *in, const char *fmt) {
  int r_std = 0, r_s21 = 0;
  int n_std = -1, n_s21 = -1;
  int v_std = 0, v_s21 = 0;

  r_std = sscanf(in, fmt, &n_std, &v_std);
  r_s21 = s21_sscanf(in, fmt, &n_s21, &v_s21);

  if (r_std == r_s21 && n_std == n_s21 && v_std == v_s21) s21t_ok(st);
  else {
    s21t_fail(st);
    s21t_fail_header(name, in, fmt);
    printf("  std: r=%d n=%d v=%d\n", r_std, n_std, v_std);
    printf("  s21: r=%d n=%d v=%d\n", r_s21, n_s21, v_s21);
  }
}

static void test_int_then_n_then_int(s21t_stats *st, const char *name, const char *in, const char *fmt) {
  int r_std = 0, r_s21 = 0;
  int a_std = 0, a_s21 = 0;
  int n_std = -1, n_s21 = -1;
  int b_std = 0, b_s21 = 0;

  r_std = sscanf(in, fmt, &a_std, &n_std, &b_std);
  r_s21 = s21_sscanf(in, fmt, &a_s21, &n_s21, &b_s21);

  if (r_std == r_s21 && a_std == a_s21 && n_std == n_s21 && b_std == b_s21) s21t_ok(st);
  else {
    s21t_fail(st);
    s21t_fail_header(name, in, fmt);
    printf("  std: r=%d a=%d n=%d b=%d\n", r_std, a_std, n_std, b_std);
    printf("  s21: r=%d a=%d n=%d b=%d\n", r_s21, a_s21, n_s21, b_s21);
  }
}

static void test_mix_2ints(s21t_stats *st, const char *name, const char *in, const char *fmt) {
  int r_std = 0, r_s21 = 0;
  int a_std = 0, b_std = 0;
  int a_s21 = 0, b_s21 = 0;

  r_std = sscanf(in, fmt, &a_std, &b_std);
  r_s21 = s21_sscanf(in, fmt, &a_s21, &b_s21);

  if (r_std == r_s21 && a_std == a_s21 && b_std == b_s21) s21t_ok(st);
  else {
    s21t_fail(st);
    s21t_fail_header(name, in, fmt);
    printf("  std: r=%d a=%d b=%d\n", r_std, a_std, b_std);
    printf("  s21: r=%d a=%d b=%d\n", r_s21, a_s21, b_s21);
  }
}

static void test_suppressed_then_int(s21t_stats *st, const char *name, const char *in, const char *fmt) {
  int r_std = 0, r_s21 = 0;
  int x_std = 0, x_s21 = 0;

  r_std = sscanf(in, fmt, &x_std);
  r_s21 = s21_sscanf(in, fmt, &x_s21);

  if (r_std == r_s21 && x_std == x_s21) s21t_ok(st);
  else {
    s21t_fail(st);
    s21t_fail_header(name, in, fmt);
    printf("  std: r=%d x=%d\n", r_std, x_std);
    printf("  s21: r=%d x=%d\n", r_s21, x_s21);
  }
}

static void test_scanset_str(s21t_stats *st, const char *name, const char *in, const char *fmt) {
  int r_std = 0, r_s21 = 0;
  char a_std[128];
  char a_s21[128];
  int i = 0;

  while (i < 128) {
    a_std[i] = 0;
    a_s21[i] = 0;
    i++;
  }

  r_std = sscanf(in, fmt, a_std);
  r_s21 = s21_sscanf(in, fmt, a_s21);

  if (r_std == r_s21 && s21t_streq(a_std, a_s21)) s21t_ok(st);
  else {
    s21t_fail(st);
    s21t_fail_header(name, in, fmt);
    printf("  std: r=%d a=\"%s\"\n", r_std, a_std);
    printf("  s21: r=%d a=\"%s\"\n", r_s21, a_s21);
  }
}

int main(void) {
  s21t_stats st;
  st.total = 0;
  st.passed = 0;

  (void)s21t_strlen;
  (void)s21t_iabs;

  printf("Running s21_sscanf smoke tests...\n");

  test_int(&st, "d basic", "123", "%d");
  test_int(&st, "d leading spaces", "   123", "%d");
  test_int(&st, "d sign -", "-42", "%d");
  test_int(&st, "d sign +", "+42", "%d");
  test_int(&st, "d width", "12345", "%3d");
  test_int(&st, "d stop at non-digit", "12x34", "%d");
  test_int(&st, "d mismatch", "x12", "%d");
  test_int(&st, "d empty", "", "%d");

  test_int(&st, "i base 10", "17", "%i");
  test_int(&st, "i base 8", "017", "%i");
  test_int(&st, "i base 16 lower", "0x1f", "%i");
  test_int(&st, "i base 16 upper", "0X1F", "%i");
  test_int(&st, "i base 16 width", "0x1234", "%4i");
  test_int(&st, "i base 0 only", "0", "%i");
  test_int(&st, "i sign and base", "-0x20", "%i");

  test_short(&st, "hd basic", "32000", "%hd");
  test_char_len(&st, "hhd basic", "127", "%hhd");
  test_ll(&st, "lld basic", "9223372036854775807", "%lld");
  test_ll(&st, "lli hex auto", "0x7f", "%lli");

  test_uint(&st, "u basic", "42", "%u");
  test_uint(&st, "u with -", "-1", "%u");
  test_uint(&st, "o basic", "755", "%o");
  test_uint(&st, "x basic", "ff", "%x");
  test_uint(&st, "X basic", "FF", "%X");
  test_uint(&st, "x with 0x", "0x10", "%x");
  test_uint(&st, "x width", "0x1234", "%3x");
  test_ull(&st, "llx big", "FFFFFFFFFFFFFFFF", "%llx");

  test_ptr(&st, "p 0x prefix", "0x7ffc", "%p");
  test_ptr(&st, "p no prefix", "7ffc", "%p");
  test_ptr(&st, "p mismatch", "zz", "%p");

  test_str(&st, "s basic", "hello world", "%s");
  test_str(&st, "s leading spaces", "   hello", "%s");
  test_str(&st, "s width", "abcdef", "%3s");
  test_str(&st, "s stop at whitespace", "abc\tdef", "%s");
  test_str(&st, "s empty input", "", "%s");

  test_c1(&st, "c basic", "A", "%c");
  test_c1(&st, "c reads space", " A", "%c");
  test_cN(&st, "c width 3", "ABCDE", "%3c", 3);
  test_cN(&st, "c width 5 exact", "ABCDE", "%5c", 5);
  test_cN(&st, "c width 2 with spaces", " A B", "%2c", 2);

  test_scanset_str(&st, "scanset letters", "abc123", "%[a-z]");
  test_scanset_str(&st, "scanset invert digits", "abc123", "%[^0-9]");
  test_scanset_str(&st, "scanset digits", "123abc", "%[0-9]");
  test_scanset_str(&st, "scanset with dash literal", "-=abc", "%[-=a-z]");
  test_scanset_str(&st, "scanset ']' first", "]abc", "%[]a-z]");
  test_scanset_str(&st, "scanset width", "abcdef", "%3[a-z]");

  test_int(&st, "percent literal then int", "%123", "%%%d");
  test_int(&st, "percent mismatch", "x123", "%%%d");

  test_suppressed_then_int(&st, "suppress int then int", "11 22", "%*d %d");
  test_suppressed_then_int(&st, "suppress string then int", "abc 77", "%*s %d");
  test_suppressed_then_int(&st, "suppress scanset then int", "abc123 9", "%*[a-z0-9] %d");

  test_mix_2ints(&st, "mix literals commas", "12,34", "%d,%d");
  test_mix_2ints(&st, "mix spaces", "12  34", "%d %d");
  test_mix_2ints(&st, "mix tabs", "12\t34", "%d%d");
  test_mix_2ints(&st, "mix mismatch second", "12,x", "%d,%d");

  test_n_then_int(&st, "n after spaces then int", "   123", "%n%d");
  test_n_then_int(&st, "n after literal", "ab12", "ab%n%d");
  test_int_then_n_then_int(&st, "n after int and space", "12 34", "%d %n%d");

  test_float_f(&st, "f basic", "3.14", "%f");
  test_float_d(&st, "lf basic", "3.14", "%lf");
  test_float_d(&st, "e exponent", "1.25e3", "%le");
  test_float_d(&st, "E exponent +", "1.25E+3", "%lE");
  test_float_d(&st, "g basic", "0.000125", "%lg");
  test_float_d(&st, "float sign", "-0.5", "%lf");
  test_float_d(&st, "float leading spaces", "   2.5", "%lf");
  test_float_d(&st, "float width", "123.456", "%4lf");
  test_float_d(&st, "float exponent rollback", "1eX", "%lf");

  test_float_d(&st, "float inf", "inf", "%lf");
  test_float_d(&st, "float infinity", "infinity", "%lf");
  test_float_d(&st, "float nan", "nan", "%lf");
  test_float_d(&st, "float nan(payload)", "nan(abc)", "%lf");

  test_int(&st, "edge only spaces", "   ", "%d");
  test_str(&st, "edge only spaces string", "   ", "%s");
  test_mix_2ints(&st, "edge first ok second EOF", "12", "%d%d");

  printf("\nSummary: %d/%d passed, %d failed\n",
         st.passed, st.total, st.total - st.passed);

  return (st.total == st.passed) ? 0 : 1;
}
