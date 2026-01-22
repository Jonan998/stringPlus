// tests_sscanf.c
#include <math.h>
#include <stdint.h>
#include <stdio.h>

#include "tests.h"

static int s21t_streq(const char *a, const char *b) {
  int eq = 1;
  int i = 0;
  if (a == NULL || b == NULL) return (a == b);
  while (eq && (a[i] != '\0' || b[i] != '\0')) {
    if (a[i] != b[i]) eq = 0;
    i++;
  }
  return eq;
}

static int s21t_float_eq(double a, double b) {
  if (isnan(a) && isnan(b)) return 1;
  if (isinf(a) && isinf(b) && ((a > 0) == (b > 0))) return 1;
  double diff = fabs(a - b);
  double scale = fmax(1.0, fmax(fabs(a), fabs(b)));
  return (diff <= 1e-9 * scale);
}

/* --- %d --- */
START_TEST(sscanf_001_d_basic) {
  const char *in = "123";
  int r1 = 0, r2 = 0;
  int a1 = 777, a2 = 777;

  r1 = sscanf(in, "%d", &a1);
  r2 = s21_sscanf(in, "%d", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

START_TEST(sscanf_002_d_leading_spaces) {
  const char *in = "   123";
  int r1 = 0, r2 = 0;
  int a1 = 777, a2 = 777;

  r1 = sscanf(in, "%d", &a1);
  r2 = s21_sscanf(in, "%d", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

START_TEST(sscanf_003_d_sign_minus) {
  const char *in = "-42";
  int r1 = 0, r2 = 0;
  int a1 = 777, a2 = 777;

  r1 = sscanf(in, "%d", &a1);
  r2 = s21_sscanf(in, "%d", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

START_TEST(sscanf_004_d_sign_plus) {
  const char *in = "+42";
  int r1 = 0, r2 = 0;
  int a1 = 777, a2 = 777;

  r1 = sscanf(in, "%d", &a1);
  r2 = s21_sscanf(in, "%d", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

START_TEST(sscanf_005_d_width) {
  const char *in = "12345";
  int r1 = 0, r2 = 0;
  int a1 = 777, a2 = 777;

  r1 = sscanf(in, "%3d", &a1);
  r2 = s21_sscanf(in, "%3d", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

START_TEST(sscanf_006_d_stop_at_nondigit) {
  const char *in = "12x34";
  int r1 = 0, r2 = 0;
  int a1 = 777, a2 = 777;

  r1 = sscanf(in, "%d", &a1);
  r2 = s21_sscanf(in, "%d", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

START_TEST(sscanf_007_d_mismatch) {
  const char *in = "x12";
  int r1 = 0, r2 = 0;
  int a1 = 777, a2 = 777;

  r1 = sscanf(in, "%d", &a1);
  r2 = s21_sscanf(in, "%d", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

START_TEST(sscanf_008_d_empty) {
  const char *in = "";
  int r1 = 0, r2 = 0;
  int a1 = 777, a2 = 777;

  r1 = sscanf(in, "%d", &a1);
  r2 = s21_sscanf(in, "%d", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

/* --- %i --- */
START_TEST(sscanf_009_i_base10) {
  const char *in = "17";
  int r1 = 0, r2 = 0;
  int a1 = 777, a2 = 777;

  r1 = sscanf(in, "%i", &a1);
  r2 = s21_sscanf(in, "%i", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

START_TEST(sscanf_010_i_base8) {
  const char *in = "017";
  int r1 = 0, r2 = 0;
  int a1 = 777, a2 = 777;

  r1 = sscanf(in, "%i", &a1);
  r2 = s21_sscanf(in, "%i", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

START_TEST(sscanf_011_i_base16_lower) {
  const char *in = "0x1f";
  int r1 = 0, r2 = 0;
  int a1 = 777, a2 = 777;

  r1 = sscanf(in, "%i", &a1);
  r2 = s21_sscanf(in, "%i", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

START_TEST(sscanf_012_i_base16_upper) {
  const char *in = "0X1F";
  int r1 = 0, r2 = 0;
  int a1 = 777, a2 = 777;

  r1 = sscanf(in, "%i", &a1);
  r2 = s21_sscanf(in, "%i", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

START_TEST(sscanf_013_i_base16_width) {
  const char *in = "0x1234";
  int r1 = 0, r2 = 0;
  int a1 = 777, a2 = 777;

  r1 = sscanf(in, "%4i", &a1);
  r2 = s21_sscanf(in, "%4i", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

START_TEST(sscanf_014_i_zero_only) {
  const char *in = "0";
  int r1 = 0, r2 = 0;
  int a1 = 777, a2 = 777;

  r1 = sscanf(in, "%i", &a1);
  r2 = s21_sscanf(in, "%i", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

START_TEST(sscanf_015_i_sign_and_base) {
  const char *in = "-0x20";
  int r1 = 0, r2 = 0;
  int a1 = 777, a2 = 777;

  r1 = sscanf(in, "%i", &a1);
  r2 = s21_sscanf(in, "%i", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

/* --- length modifiers for integers --- */
START_TEST(sscanf_016_hd_basic) {
  const char *in = "32000";
  int r1 = 0, r2 = 0;
  short a1 = (short)-1234, a2 = (short)-1234;

  r1 = sscanf(in, "%hd", &a1);
  r2 = s21_sscanf(in, "%hd", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq((int)a1, (int)a2);
}
END_TEST

START_TEST(sscanf_017_hhd_basic) {
  const char *in = "127";
  int r1 = 0, r2 = 0;
  signed char a1 = (signed char)55, a2 = (signed char)55;

  r1 = sscanf(in, "%hhd", &a1);
  r2 = s21_sscanf(in, "%hhd", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq((int)a1, (int)a2);
}
END_TEST

START_TEST(sscanf_018_lld_basic) {
  const char *in = "9223372036854775807";
  int r1 = 0, r2 = 0;
  long long a1 = 0x1122334455667788LL, a2 = 0x1122334455667788LL;

  r1 = sscanf(in, "%lld", &a1);
  r2 = s21_sscanf(in, "%lld", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq((long long)a1, (long long)a2);
}
END_TEST

START_TEST(sscanf_019_lli_hex_auto) {
  const char *in = "0x7f";
  int r1 = 0, r2 = 0;
  long long a1 = 0x1122334455667788LL, a2 = 0x1122334455667788LL;

  r1 = sscanf(in, "%lli", &a1);
  r2 = s21_sscanf(in, "%lli", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq((long long)a1, (long long)a2);
}
END_TEST

/* --- unsigned: %u %o %x %X and long long variants --- */
START_TEST(sscanf_020_u_basic) {
  const char *in = "42";
  int r1 = 0, r2 = 0;
  unsigned int a1 = 0xDEADBEEFu, a2 = 0xDEADBEEFu;

  r1 = sscanf(in, "%u", &a1);
  r2 = s21_sscanf(in, "%u", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_uint_eq(a1, a2);
}
END_TEST

START_TEST(sscanf_021_u_with_minus) {
  const char *in = "-1";
  int r1 = 0, r2 = 0;
  unsigned int a1 = 0xDEADBEEFu, a2 = 0xDEADBEEFu;

  r1 = sscanf(in, "%u", &a1);
  r2 = s21_sscanf(in, "%u", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_uint_eq(a1, a2);
}
END_TEST

START_TEST(sscanf_022_o_basic) {
  const char *in = "755";
  int r1 = 0, r2 = 0;
  unsigned int a1 = 0xDEADBEEFu, a2 = 0xDEADBEEFu;

  r1 = sscanf(in, "%o", &a1);
  r2 = s21_sscanf(in, "%o", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_uint_eq(a1, a2);
}
END_TEST

START_TEST(sscanf_023_x_basic) {
  const char *in = "ff";
  int r1 = 0, r2 = 0;
  unsigned int a1 = 0xDEADBEEFu, a2 = 0xDEADBEEFu;

  r1 = sscanf(in, "%x", &a1);
  r2 = s21_sscanf(in, "%x", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_uint_eq(a1, a2);
}
END_TEST

START_TEST(sscanf_024_X_basic) {
  const char *in = "FF";
  int r1 = 0, r2 = 0;
  unsigned int a1 = 0xDEADBEEFu, a2 = 0xDEADBEEFu;

  r1 = sscanf(in, "%X", &a1);
  r2 = s21_sscanf(in, "%X", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_uint_eq(a1, a2);
}
END_TEST

START_TEST(sscanf_025_x_with_0x) {
  const char *in = "0x10";
  int r1 = 0, r2 = 0;
  unsigned int a1 = 0xDEADBEEFu, a2 = 0xDEADBEEFu;

  r1 = sscanf(in, "%x", &a1);
  r2 = s21_sscanf(in, "%x", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_uint_eq(a1, a2);
}
END_TEST

START_TEST(sscanf_026_x_width) {
  const char *in = "0x1234";
  int r1 = 0, r2 = 0;
  unsigned int a1 = 0xDEADBEEFu, a2 = 0xDEADBEEFu;

  r1 = sscanf(in, "%3x", &a1);
  r2 = s21_sscanf(in, "%3x", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_uint_eq(a1, a2);
}
END_TEST

START_TEST(sscanf_027_llx_big) {
  const char *in = "FFFFFFFFFFFFFFFF";
  int r1 = 0, r2 = 0;
  unsigned long long a1 = 0x1122334455667788ULL, a2 = 0x1122334455667788ULL;

  r1 = sscanf(in, "%llx", &a1);
  r2 = s21_sscanf(in, "%llx", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_uint_eq((unsigned long long)a1, (unsigned long long)a2);
}
END_TEST

/* --- %p --- */
START_TEST(sscanf_028_p_0x_prefix) {
  const char *in = "0x7ffc";
  int r1 = 0, r2 = 0;
  void *p1 = (void *)(uintptr_t)0x11111111u;
  void *p2 = (void *)(uintptr_t)0x11111111u;

  r1 = sscanf(in, "%p", &p1);
  r2 = s21_sscanf(in, "%p", &p2);

  ck_assert_int_eq(r1, r2);
  ck_assert_uint_eq((uintptr_t)p1, (uintptr_t)p2);
}
END_TEST

START_TEST(sscanf_029_p_no_prefix) {
  const char *in = "7ffc";
  int r1 = 0, r2 = 0;
  void *p1 = (void *)(uintptr_t)0x11111111u;
  void *p2 = (void *)(uintptr_t)0x11111111u;

  r1 = sscanf(in, "%p", &p1);
  r2 = s21_sscanf(in, "%p", &p2);

  ck_assert_int_eq(r1, r2);
  ck_assert_uint_eq((uintptr_t)p1, (uintptr_t)p2);
}
END_TEST

START_TEST(sscanf_030_p_mismatch) {
  const char *in = "zz";
  int r1 = 0, r2 = 0;
  void *p1 = (void *)(uintptr_t)0x11111111u;
  void *p2 = (void *)(uintptr_t)0x11111111u;

  r1 = sscanf(in, "%p", &p1);
  r2 = s21_sscanf(in, "%p", &p2);

  ck_assert_int_eq(r1, r2);
  ck_assert_uint_eq((uintptr_t)p1, (uintptr_t)p2);
}
END_TEST

/* --- %s --- */
START_TEST(sscanf_031_s_basic) {
  const char *in = "hello world";
  int r1 = 0, r2 = 0;
  char a1[128] = {0};
  char a2[128] = {0};

  r1 = sscanf(in, "%s", a1);
  r2 = s21_sscanf(in, "%s", a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(s21t_streq(a1, a2), 1);
}
END_TEST

START_TEST(sscanf_032_s_leading_spaces) {
  const char *in = "   hello";
  int r1 = 0, r2 = 0;
  char a1[128] = {0};
  char a2[128] = {0};

  r1 = sscanf(in, "%s", a1);
  r2 = s21_sscanf(in, "%s", a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(s21t_streq(a1, a2), 1);
}
END_TEST

START_TEST(sscanf_033_s_width) {
  const char *in = "abcdef";
  int r1 = 0, r2 = 0;
  char a1[128] = {0};
  char a2[128] = {0};

  r1 = sscanf(in, "%3s", a1);
  r2 = s21_sscanf(in, "%3s", a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(s21t_streq(a1, a2), 1);
}
END_TEST

START_TEST(sscanf_034_s_stop_at_whitespace) {
  const char *in = "abc\tdef";
  int r1 = 0, r2 = 0;
  char a1[128] = {0};
  char a2[128] = {0};

  r1 = sscanf(in, "%s", a1);
  r2 = s21_sscanf(in, "%s", a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(s21t_streq(a1, a2), 1);
}
END_TEST

START_TEST(sscanf_035_s_empty_input) {
  const char *in = "";
  int r1 = 0, r2 = 0;
  char a1[128] = {0};
  char a2[128] = {0};

  r1 = sscanf(in, "%s", a1);
  r2 = s21_sscanf(in, "%s", a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(s21t_streq(a1, a2), 1);
}
END_TEST

/* --- %c and width for %c --- */
START_TEST(sscanf_036_c_basic) {
  const char *in = "A";
  int r1 = 0, r2 = 0;
  char c1 = 0, c2 = 0;

  r1 = sscanf(in, "%c", &c1);
  r2 = s21_sscanf(in, "%c", &c2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq((unsigned char)c1, (unsigned char)c2);
}
END_TEST

START_TEST(sscanf_037_c_reads_space) {
  const char *in = " A";
  int r1 = 0, r2 = 0;
  char c1 = 0, c2 = 0;

  r1 = sscanf(in, "%c", &c1);
  r2 = s21_sscanf(in, "%c", &c2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq((unsigned char)c1, (unsigned char)c2);
}
END_TEST

START_TEST(sscanf_038_c_width_3) {
  const char *in = "ABCDE";
  int r1 = 0, r2 = 0;
  unsigned char b1[64];
  unsigned char b2[64];

  for (int i = 0; i < 64; i++) b1[i] = b2[i] = 0xAA;

  r1 = sscanf(in, "%3c", (char *)b1);
  r2 = s21_sscanf(in, "%3c", (char *)b2);

  ck_assert_int_eq(r1, r2);
  for (int i = 0; i < 3; i++) ck_assert_int_eq((int)b1[i], (int)b2[i]);
}
END_TEST

START_TEST(sscanf_039_c_width_5_exact) {
  const char *in = "ABCDE";
  int r1 = 0, r2 = 0;
  unsigned char b1[64];
  unsigned char b2[64];

  for (int i = 0; i < 64; i++) b1[i] = b2[i] = 0xAA;

  r1 = sscanf(in, "%5c", (char *)b1);
  r2 = s21_sscanf(in, "%5c", (char *)b2);

  ck_assert_int_eq(r1, r2);
  for (int i = 0; i < 5; i++) ck_assert_int_eq((int)b1[i], (int)b2[i]);
}
END_TEST

START_TEST(sscanf_040_c_width_2_with_spaces) {
  const char *in = " A B";
  int r1 = 0, r2 = 0;
  unsigned char b1[64];
  unsigned char b2[64];

  for (int i = 0; i < 64; i++) b1[i] = b2[i] = 0xAA;

  r1 = sscanf(in, "%2c", (char *)b1);
  r2 = s21_sscanf(in, "%2c", (char *)b2);

  ck_assert_int_eq(r1, r2);
  for (int i = 0; i < 2; i++) ck_assert_int_eq((int)b1[i], (int)b2[i]);
}
END_TEST

/* --- scanset %[...], %[^...] --- */
START_TEST(sscanf_041_scanset_letters) {
  const char *in = "abc123";
  int r1 = 0, r2 = 0;
  char a1[128] = {0};
  char a2[128] = {0};

  r1 = sscanf(in, "%[a-z]", a1);
  r2 = s21_sscanf(in, "%[a-z]", a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(s21t_streq(a1, a2), 1);
}
END_TEST

START_TEST(sscanf_042_scanset_invert_digits) {
  const char *in = "abc123";
  int r1 = 0, r2 = 0;
  char a1[128] = {0};
  char a2[128] = {0};

  r1 = sscanf(in, "%[^0-9]", a1);
  r2 = s21_sscanf(in, "%[^0-9]", a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(s21t_streq(a1, a2), 1);
}
END_TEST

START_TEST(sscanf_043_scanset_digits) {
  const char *in = "123abc";
  int r1 = 0, r2 = 0;
  char a1[128] = {0};
  char a2[128] = {0};

  r1 = sscanf(in, "%[0-9]", a1);
  r2 = s21_sscanf(in, "%[0-9]", a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(s21t_streq(a1, a2), 1);
}
END_TEST

START_TEST(sscanf_044_scanset_dash_literal) {
  const char *in = "-=abc";
  int r1 = 0, r2 = 0;
  char a1[128] = {0};
  char a2[128] = {0};

  r1 = sscanf(in, "%[-=a-z]", a1);
  r2 = s21_sscanf(in, "%[-=a-z]", a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(s21t_streq(a1, a2), 1);
}
END_TEST

START_TEST(sscanf_045_scanset_bracket_first) {
  const char *in = "]abc";
  int r1 = 0, r2 = 0;
  char a1[128] = {0};
  char a2[128] = {0};

  r1 = sscanf(in, "%[]a-z]", a1);
  r2 = s21_sscanf(in, "%[]a-z]", a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(s21t_streq(a1, a2), 1);
}
END_TEST

START_TEST(sscanf_046_scanset_width) {
  const char *in = "abcdef";
  int r1 = 0, r2 = 0;
  char a1[128] = {0};
  char a2[128] = {0};

  r1 = sscanf(in, "%3[a-z]", a1);
  r2 = s21_sscanf(in, "%3[a-z]", a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(s21t_streq(a1, a2), 1);
}
END_TEST

/* --- literal %%, then conversion --- */
START_TEST(sscanf_047_percent_literal_then_int) {
  const char *in = "%123";
  int r1 = 0, r2 = 0;
  int a1 = 777, a2 = 777;

  r1 = sscanf(in, "%%%d", &a1);
  r2 = s21_sscanf(in, "%%%d", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

START_TEST(sscanf_048_percent_mismatch) {
  const char *in = "x123";
  int r1 = 0, r2 = 0;
  int a1 = 777, a2 = 777;

  r1 = sscanf(in, "%%%d", &a1);
  r2 = s21_sscanf(in, "%%%d", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

/* --- suppression %* --- */
START_TEST(sscanf_049_suppress_int_then_int) {
  const char *in = "11 22";
  int r1 = 0, r2 = 0;
  int x1 = 777, x2 = 777;

  r1 = sscanf(in, "%*d %d", &x1);
  r2 = s21_sscanf(in, "%*d %d", &x2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(x1, x2);
}
END_TEST

START_TEST(sscanf_050_suppress_string_then_int) {
  const char *in = "abc 77";
  int r1 = 0, r2 = 0;
  int x1 = 777, x2 = 777;

  r1 = sscanf(in, "%*s %d", &x1);
  r2 = s21_sscanf(in, "%*s %d", &x2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(x1, x2);
}
END_TEST

START_TEST(sscanf_051_suppress_scanset_then_int) {
  const char *in = "abc123 9";
  int r1 = 0, r2 = 0;
  int x1 = 777, x2 = 777;

  r1 = sscanf(in, "%*[a-z0-9] %d", &x1);
  r2 = s21_sscanf(in, "%*[a-z0-9] %d", &x2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(x1, x2);
}
END_TEST

/* --- mix formats --- */
START_TEST(sscanf_052_mix_literals_commas) {
  const char *in = "12,34";
  int r1 = 0, r2 = 0;
  int a1 = 0, b1 = 0;
  int a2 = 0, b2 = 0;

  r1 = sscanf(in, "%d,%d", &a1, &b1);
  r2 = s21_sscanf(in, "%d,%d", &a2, &b2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(a1, a2);
  ck_assert_int_eq(b1, b2);
}
END_TEST

START_TEST(sscanf_053_mix_spaces) {
  const char *in = "12  34";
  int r1 = 0, r2 = 0;
  int a1 = 0, b1 = 0;
  int a2 = 0, b2 = 0;

  r1 = sscanf(in, "%d %d", &a1, &b1);
  r2 = s21_sscanf(in, "%d %d", &a2, &b2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(a1, a2);
  ck_assert_int_eq(b1, b2);
}
END_TEST

START_TEST(sscanf_054_mix_tabs_fmt_dd) {
  const char *in = "12\t34";
  int r1 = 0, r2 = 0;
  int a1 = 0, b1 = 0;
  int a2 = 0, b2 = 0;

  r1 = sscanf(in, "%d%d", &a1, &b1);
  r2 = s21_sscanf(in, "%d%d", &a2, &b2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(a1, a2);
  ck_assert_int_eq(b1, b2);
}
END_TEST

START_TEST(sscanf_055_mix_mismatch_second) {
  const char *in = "12,x";
  int r1 = 0, r2 = 0;
  int a1 = 0, b1 = 0;
  int a2 = 0, b2 = 0;

  r1 = sscanf(in, "%d,%d", &a1, &b1);
  r2 = s21_sscanf(in, "%d,%d", &a2, &b2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(a1, a2);
  ck_assert_int_eq(b1, b2);
}
END_TEST

/* --- %n cases --- */
START_TEST(sscanf_056_n_after_spaces_then_int) {
  const char *in = "   123";
  int r1 = 0, r2 = 0;
  int n1 = -1, n2 = -1;
  int v1 = 0, v2 = 0;

  r1 = sscanf(in, "%n%d", &n1, &v1);
  r2 = s21_sscanf(in, "%n%d", &n2, &v2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(n1, n2);
  ck_assert_int_eq(v1, v2);
}
END_TEST

START_TEST(sscanf_057_n_after_literal) {
  const char *in = "ab12";
  int r1 = 0, r2 = 0;
  int n1 = -1, n2 = -1;
  int v1 = 0, v2 = 0;

  r1 = sscanf(in, "ab%n%d", &n1, &v1);
  r2 = s21_sscanf(in, "ab%n%d", &n2, &v2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(n1, n2);
  ck_assert_int_eq(v1, v2);
}
END_TEST

START_TEST(sscanf_058_int_then_n_then_int) {
  const char *in = "12 34";
  int r1 = 0, r2 = 0;
  int a1 = 0, a2 = 0;
  int n1 = -1, n2 = -1;
  int b1 = 0, b2 = 0;

  r1 = sscanf(in, "%d %n%d", &a1, &n1, &b1);
  r2 = s21_sscanf(in, "%d %n%d", &a2, &n2, &b2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(a1, a2);
  ck_assert_int_eq(n1, n2);
  ck_assert_int_eq(b1, b2);
}
END_TEST

/* --- floats --- */
START_TEST(sscanf_059_f_basic) {
  const char *in = "3.14";
  int r1 = 0, r2 = 0;
  float a1 = 12.5f, a2 = 12.5f;

  r1 = sscanf(in, "%f", &a1);
  r2 = s21_sscanf(in, "%f", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(s21t_float_eq((double)a1, (double)a2), 1);
}
END_TEST

START_TEST(sscanf_060_lf_basic) {
  const char *in = "3.14";
  int r1 = 0, r2 = 0;
  double a1 = 123.456, a2 = 123.456;

  r1 = sscanf(in, "%lf", &a1);
  r2 = s21_sscanf(in, "%lf", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(s21t_float_eq(a1, a2), 1);
}
END_TEST

START_TEST(sscanf_061_e_exponent) {
  const char *in = "1.25e3";
  int r1 = 0, r2 = 0;
  double a1 = 123.456, a2 = 123.456;

  r1 = sscanf(in, "%le", &a1);
  r2 = s21_sscanf(in, "%le", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(s21t_float_eq(a1, a2), 1);
}
END_TEST

START_TEST(sscanf_062_E_exponent_plus) {
  const char *in = "1.25E+3";
  int r1 = 0, r2 = 0;
  double a1 = 123.456, a2 = 123.456;

  r1 = sscanf(in, "%lE", &a1);
  r2 = s21_sscanf(in, "%lE", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(s21t_float_eq(a1, a2), 1);
}
END_TEST

START_TEST(sscanf_063_g_basic) {
  const char *in = "0.000125";
  int r1 = 0, r2 = 0;
  double a1 = 123.456, a2 = 123.456;

  r1 = sscanf(in, "%lg", &a1);
  r2 = s21_sscanf(in, "%lg", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(s21t_float_eq(a1, a2), 1);
}
END_TEST

START_TEST(sscanf_064_float_sign) {
  const char *in = "-0.5";
  int r1 = 0, r2 = 0;
  double a1 = 123.456, a2 = 123.456;

  r1 = sscanf(in, "%lf", &a1);
  r2 = s21_sscanf(in, "%lf", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(s21t_float_eq(a1, a2), 1);
}
END_TEST

START_TEST(sscanf_065_float_leading_spaces) {
  const char *in = "   2.5";
  int r1 = 0, r2 = 0;
  double a1 = 123.456, a2 = 123.456;

  r1 = sscanf(in, "%lf", &a1);
  r2 = s21_sscanf(in, "%lf", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(s21t_float_eq(a1, a2), 1);
}
END_TEST

START_TEST(sscanf_066_float_width) {
  const char *in = "123.456";
  int r1 = 0, r2 = 0;
  double a1 = 123.456, a2 = 123.456;

  r1 = sscanf(in, "%4lf", &a1);
  r2 = s21_sscanf(in, "%4lf", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(s21t_float_eq(a1, a2), 1);
}
END_TEST

START_TEST(sscanf_067_float_exponent_rollback) {
  const char *in = "1eX";
  int r1 = 0, r2 = 0;
  double a1 = 123.456, a2 = 123.456;

  r1 = sscanf(in, "%lf", &a1);
  r2 = s21_sscanf(in, "%lf", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(s21t_float_eq(a1, a2), 1);
}
END_TEST

START_TEST(sscanf_068_float_inf) {
  const char *in = "inf";
  int r1 = 0, r2 = 0;
  double a1 = 123.456, a2 = 123.456;

  r1 = sscanf(in, "%lf", &a1);
  r2 = s21_sscanf(in, "%lf", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(s21t_float_eq(a1, a2), 1);
}
END_TEST

START_TEST(sscanf_069_float_infinity) {
  const char *in = "infinity";
  int r1 = 0, r2 = 0;
  double a1 = 123.456, a2 = 123.456;

  r1 = sscanf(in, "%lf", &a1);
  r2 = s21_sscanf(in, "%lf", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(s21t_float_eq(a1, a2), 1);
}
END_TEST

START_TEST(sscanf_070_float_nan) {
  const char *in = "nan";
  int r1 = 0, r2 = 0;
  double a1 = 123.456, a2 = 123.456;

  r1 = sscanf(in, "%lf", &a1);
  r2 = s21_sscanf(in, "%lf", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(s21t_float_eq(a1, a2), 1);
}
END_TEST

START_TEST(sscanf_071_float_nan_payload) {
  const char *in = "nan(abc)";
  int r1 = 0, r2 = 0;
  double a1 = 123.456, a2 = 123.456;

  r1 = sscanf(in, "%lf", &a1);
  r2 = s21_sscanf(in, "%lf", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(s21t_float_eq(a1, a2), 1);
}
END_TEST

/* --- edge cases from your list --- */
START_TEST(sscanf_072_edge_only_spaces_int) {
  const char *in = "   ";
  int r1 = 0, r2 = 0;
  int a1 = 777, a2 = 777;

  r1 = sscanf(in, "%d", &a1);
  r2 = s21_sscanf(in, "%d", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

START_TEST(sscanf_073_edge_only_spaces_string) {
  const char *in = "   ";
  int r1 = 0, r2 = 0;
  char a1[128] = {0};
  char a2[128] = {0};

  r1 = sscanf(in, "%s", a1);
  r2 = s21_sscanf(in, "%s", a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(s21t_streq(a1, a2), 1);
}
END_TEST

START_TEST(sscanf_074_edge_first_ok_second_eof) {
  const char *in = "12";
  int r1 = 0, r2 = 0;
  int a1 = 0, b1 = 777;
  int a2 = 0, b2 = 777;

  r1 = sscanf(in, "%d%d", &a1, &b1);
  r2 = s21_sscanf(in, "%d%d", &a2, &b2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(a1, a2);
  ck_assert_int_eq(b1, b2);
}
END_TEST

/* -------------------- additional tests to reach 100 -------------------- */

/* 75 */
START_TEST(sscanf_075_d_trailing_spaces) {
  const char *in = "123   ";
  int r1 = 0, r2 = 0;
  int a1 = 777, a2 = 777;

  r1 = sscanf(in, "%d", &a1);
  r2 = s21_sscanf(in, "%d", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

/* 76 */
START_TEST(sscanf_076_d_leading_newline) {
  const char *in = "\n42";
  int r1 = 0, r2 = 0;
  int a1 = 777, a2 = 777;

  r1 = sscanf(in, "%d", &a1);
  r2 = s21_sscanf(in, "%d", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

/* 77 */
START_TEST(sscanf_077_d_plus_width) {
  const char *in = "+12345";
  int r1 = 0, r2 = 0;
  int a1 = 777, a2 = 777;

  r1 = sscanf(in, "%4d", &a1);
  r2 = s21_sscanf(in, "%4d", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

/* 78 */
START_TEST(sscanf_078_i_no_prefix_hex_letters_should_fail) {
  const char *in = "FF";
  int r1 = 0, r2 = 0;
  int a1 = 777, a2 = 777;

  r1 = sscanf(in, "%i", &a1);
  r2 = s21_sscanf(in, "%i", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

/* 79 */
START_TEST(sscanf_079_i_octal_invalid_digit_stops) {
  const char *in = "078";
  int r1 = 0, r2 = 0;
  int a1 = 777, a2 = 777;

  r1 = sscanf(in, "%i", &a1);
  r2 = s21_sscanf(in, "%i", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

/* 80 */
START_TEST(sscanf_080_u_leading_spaces) {
  const char *in = "   42";
  int r1 = 0, r2 = 0;
  unsigned int a1 = 0xDEADBEEFu, a2 = 0xDEADBEEFu;

  r1 = sscanf(in, "%u", &a1);
  r2 = s21_sscanf(in, "%u", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_uint_eq(a1, a2);
}
END_TEST

/* 81 */
START_TEST(sscanf_081_o_leading_zero) {
  const char *in = "0755";
  int r1 = 0, r2 = 0;
  unsigned int a1 = 0xDEADBEEFu, a2 = 0xDEADBEEFu;

  r1 = sscanf(in, "%o", &a1);
  r2 = s21_sscanf(in, "%o", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_uint_eq(a1, a2);
}
END_TEST

/* 82 */
START_TEST(sscanf_082_x_mixed_case) {
  const char *in = "aBcD";
  int r1 = 0, r2 = 0;
  unsigned int a1 = 0xDEADBEEFu, a2 = 0xDEADBEEFu;

  r1 = sscanf(in, "%x", &a1);
  r2 = s21_sscanf(in, "%x", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_uint_eq(a1, a2);
}
END_TEST

/* 83 */
START_TEST(sscanf_083_x_negative_sign) {
  const char *in = "-FF";
  int r1 = 0, r2 = 0;
  unsigned int a1 = 0xDEADBEEFu, a2 = 0xDEADBEEFu;

  r1 = sscanf(in, "%x", &a1);
  r2 = s21_sscanf(in, "%x", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_uint_eq(a1, a2);
}
END_TEST

/* 84 */
START_TEST(sscanf_084_p_zero) {
  const char *in = "0x0";
  int r1 = 0, r2 = 0;
  void *p1 = (void *)(uintptr_t)0x11111111u;
  void *p2 = (void *)(uintptr_t)0x11111111u;

  r1 = sscanf(in, "%p", &p1);
  r2 = s21_sscanf(in, "%p", &p2);

  ck_assert_int_eq(r1, r2);
  ck_assert_uint_eq((uintptr_t)p1, (uintptr_t)p2);
}
END_TEST

/* 85 */
START_TEST(sscanf_085_s_stops_at_newline) {
  const char *in = "abc\ndef";
  int r1 = 0, r2 = 0;
  char a1[128] = {0};
  char a2[128] = {0};

  r1 = sscanf(in, "%s", a1);
  r2 = s21_sscanf(in, "%s", a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(s21t_streq(a1, a2), 1);
}
END_TEST

/* 86 */
START_TEST(sscanf_086_s_leading_tabs) {
  const char *in = "\t\tabc";
  int r1 = 0, r2 = 0;
  char a1[128] = {0};
  char a2[128] = {0};

  r1 = sscanf(in, "%s", a1);
  r2 = s21_sscanf(in, "%s", a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(s21t_streq(a1, a2), 1);
}
END_TEST

/* 87 */
START_TEST(sscanf_087_s_width_with_space_after) {
  const char *in = "abc def";
  int r1 = 0, r2 = 0;
  char a1[128] = {0};
  char a2[128] = {0};

  r1 = sscanf(in, "%3s", a1);
  r2 = s21_sscanf(in, "%3s", a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(s21t_streq(a1, a2), 1);
}
END_TEST

/* 88 */
START_TEST(sscanf_088_c_newline_char) {
  const char *in = "\n";
  int r1 = 0, r2 = 0;
  char c1 = 'X', c2 = 'X';

  r1 = sscanf(in, "%c", &c1);
  r2 = s21_sscanf(in, "%c", &c2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq((unsigned char)c1, (unsigned char)c2);
}
END_TEST

/* 89 */
START_TEST(sscanf_089_space_before_c_skips_whitespace) {
  const char *in = "   Z";
  int r1 = 0, r2 = 0;
  char c1 = 'X', c2 = 'X';

  r1 = sscanf(in, " %c", &c1);
  r2 = s21_sscanf(in, " %c", &c2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq((unsigned char)c1, (unsigned char)c2);
}
END_TEST

/* 90 */
START_TEST(sscanf_090_scanset_until_space) {
  const char *in = "abc def";
  int r1 = 0, r2 = 0;
  char a1[128] = {0};
  char a2[128] = {0};

  r1 = sscanf(in, "%[^ ]", a1);
  r2 = s21_sscanf(in, "%[^ ]", a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(s21t_streq(a1, a2), 1);
}
END_TEST

/* 91 */
START_TEST(sscanf_091_scanset_invert_a_reads_spaces) {
  const char *in = "   abc";
  int r1 = 0, r2 = 0;
  char a1[128] = {0};
  char a2[128] = {0};

  r1 = sscanf(in, "%[^a]", a1);
  r2 = s21_sscanf(in, "%[^a]", a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(s21t_streq(a1, a2), 1);
}
END_TEST

/* 92 */
START_TEST(sscanf_092_percent_only_match_no_args) {
  const char *in = "%";
  int r1 = 0, r2 = 0;

  r1 = sscanf(in, "%%");
  r2 = s21_sscanf(in, "%%");

  ck_assert_int_eq(r1, r2);
}
END_TEST

/* 93 */
START_TEST(sscanf_093_percent_only_mismatch_no_args) {
  const char *in = "x";
  int r1 = 0, r2 = 0;

  r1 = sscanf(in, "%%");
  r2 = s21_sscanf(in, "%%");

  ck_assert_int_eq(r1, r2);
}
END_TEST

/* 94 */
START_TEST(sscanf_094_suppress_double_then_int) {
  const char *in = "3.14 7";
  int r1 = 0, r2 = 0;
  int x1 = 777, x2 = 777;

  r1 = sscanf(in, "%*f %d", &x1);
  r2 = s21_sscanf(in, "%*lf %d", &x2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(x1, x2);
}
END_TEST

/* 95 */
START_TEST(sscanf_095_two_ints_first_width) {
  const char *in = "12345 678";
  int r1 = 0, r2 = 0;
  int a1 = 777, b1 = 777;
  int a2 = 777, b2 = 777;

  r1 = sscanf(in, "%2d %d", &a1, &b1);
  r2 = s21_sscanf(in, "%2d %d", &a2, &b2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(a1, a2);
  ck_assert_int_eq(b1, b2);
}
END_TEST

/* 96 */
START_TEST(sscanf_096_mix_literal_comma_space) {
  const char *in = "12, 34";
  int r1 = 0, r2 = 0;
  int a1 = 0, b1 = 0;
  int a2 = 0, b2 = 0;

  r1 = sscanf(in, "%d, %d", &a1, &b1);
  r2 = s21_sscanf(in, "%d, %d", &a2, &b2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(a1, a2);
  ck_assert_int_eq(b1, b2);
}
END_TEST

/* 97 */
START_TEST(sscanf_097_n_after_suppressed_string) {
  const char *in = "abc 77";
  int r1 = 0, r2 = 0;
  int n1 = -1, n2 = -1;
  int x1 = 777, x2 = 777;

  r1 = sscanf(in, "%*s %n%d", &n1, &x1);
  r2 = s21_sscanf(in, "%*s %n%d", &n2, &x2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(n1, n2);
  ck_assert_int_eq(x1, x2);
}
END_TEST

/* 98 */
START_TEST(sscanf_098_n_only) {
  const char *in = "hello";
  int r1 = 0, r2 = 0;
  int n1 = -1, n2 = -1;

  r1 = sscanf(in, "%n", &n1);
  r2 = s21_sscanf(in, "%n", &n2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(n1, n2);
}
END_TEST

/* 99 */
START_TEST(sscanf_099_float_leading_dot) {
  const char *in = ".5";
  int r1 = 0, r2 = 0;
  double a1 = 123.456, a2 = 123.456;

  r1 = sscanf(in, "%lf", &a1);
  r2 = s21_sscanf(in, "%lf", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(s21t_float_eq(a1, a2), 1);
}
END_TEST

/* 100 */
START_TEST(sscanf_100_float_trailing_dot) {
  const char *in = "2.";
  int r1 = 0, r2 = 0;
  double a1 = 123.456, a2 = 123.456;

  r1 = sscanf(in, "%lf", &a1);
  r2 = s21_sscanf(in, "%lf", &a2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(s21t_float_eq(a1, a2), 1);
}
END_TEST

Suite *test_sscanf(void) {
  Suite *s = suite_create("\033[45m-=S21_SSCANF=-\033[0m");
  TCase *tc = tcase_create("sscanf_tc");

  tcase_add_test(tc, sscanf_001_d_basic);
  tcase_add_test(tc, sscanf_002_d_leading_spaces);
  tcase_add_test(tc, sscanf_003_d_sign_minus);
  tcase_add_test(tc, sscanf_004_d_sign_plus);
  tcase_add_test(tc, sscanf_005_d_width);
  tcase_add_test(tc, sscanf_006_d_stop_at_nondigit);
  tcase_add_test(tc, sscanf_007_d_mismatch);
  tcase_add_test(tc, sscanf_008_d_empty);

  tcase_add_test(tc, sscanf_009_i_base10);
  tcase_add_test(tc, sscanf_010_i_base8);
  tcase_add_test(tc, sscanf_011_i_base16_lower);
  tcase_add_test(tc, sscanf_012_i_base16_upper);
  tcase_add_test(tc, sscanf_013_i_base16_width);
  tcase_add_test(tc, sscanf_014_i_zero_only);
  tcase_add_test(tc, sscanf_015_i_sign_and_base);

  tcase_add_test(tc, sscanf_016_hd_basic);
  tcase_add_test(tc, sscanf_017_hhd_basic);
  tcase_add_test(tc, sscanf_018_lld_basic);
  tcase_add_test(tc, sscanf_019_lli_hex_auto);

  tcase_add_test(tc, sscanf_020_u_basic);
  tcase_add_test(tc, sscanf_021_u_with_minus);
  tcase_add_test(tc, sscanf_022_o_basic);
  tcase_add_test(tc, sscanf_023_x_basic);
  tcase_add_test(tc, sscanf_024_X_basic);
  tcase_add_test(tc, sscanf_025_x_with_0x);
  tcase_add_test(tc, sscanf_026_x_width);
  tcase_add_test(tc, sscanf_027_llx_big);

  tcase_add_test(tc, sscanf_028_p_0x_prefix);
  tcase_add_test(tc, sscanf_029_p_no_prefix);
  tcase_add_test(tc, sscanf_030_p_mismatch);

  tcase_add_test(tc, sscanf_031_s_basic);
  tcase_add_test(tc, sscanf_032_s_leading_spaces);
  tcase_add_test(tc, sscanf_033_s_width);
  tcase_add_test(tc, sscanf_034_s_stop_at_whitespace);
  tcase_add_test(tc, sscanf_035_s_empty_input);

  tcase_add_test(tc, sscanf_036_c_basic);
  tcase_add_test(tc, sscanf_037_c_reads_space);
  tcase_add_test(tc, sscanf_038_c_width_3);
  tcase_add_test(tc, sscanf_039_c_width_5_exact);
  tcase_add_test(tc, sscanf_040_c_width_2_with_spaces);

  tcase_add_test(tc, sscanf_041_scanset_letters);
  tcase_add_test(tc, sscanf_042_scanset_invert_digits);
  tcase_add_test(tc, sscanf_043_scanset_digits);
  tcase_add_test(tc, sscanf_044_scanset_dash_literal);
  tcase_add_test(tc, sscanf_045_scanset_bracket_first);
  tcase_add_test(tc, sscanf_046_scanset_width);

  tcase_add_test(tc, sscanf_047_percent_literal_then_int);
  tcase_add_test(tc, sscanf_048_percent_mismatch);

  tcase_add_test(tc, sscanf_049_suppress_int_then_int);
  tcase_add_test(tc, sscanf_050_suppress_string_then_int);
  tcase_add_test(tc, sscanf_051_suppress_scanset_then_int);

  tcase_add_test(tc, sscanf_052_mix_literals_commas);
  tcase_add_test(tc, sscanf_053_mix_spaces);
  tcase_add_test(tc, sscanf_054_mix_tabs_fmt_dd);
  tcase_add_test(tc, sscanf_055_mix_mismatch_second);

  tcase_add_test(tc, sscanf_056_n_after_spaces_then_int);
  tcase_add_test(tc, sscanf_057_n_after_literal);
  tcase_add_test(tc, sscanf_058_int_then_n_then_int);

  tcase_add_test(tc, sscanf_059_f_basic);
  tcase_add_test(tc, sscanf_060_lf_basic);
  tcase_add_test(tc, sscanf_061_e_exponent);
  tcase_add_test(tc, sscanf_062_E_exponent_plus);
  tcase_add_test(tc, sscanf_063_g_basic);
  tcase_add_test(tc, sscanf_064_float_sign);
  tcase_add_test(tc, sscanf_065_float_leading_spaces);
  tcase_add_test(tc, sscanf_066_float_width);
  tcase_add_test(tc, sscanf_067_float_exponent_rollback);
  tcase_add_test(tc, sscanf_068_float_inf);
  tcase_add_test(tc, sscanf_069_float_infinity);
  tcase_add_test(tc, sscanf_070_float_nan);
  tcase_add_test(tc, sscanf_071_float_nan_payload);

  tcase_add_test(tc, sscanf_072_edge_only_spaces_int);
  tcase_add_test(tc, sscanf_073_edge_only_spaces_string);
  tcase_add_test(tc, sscanf_074_edge_first_ok_second_eof);

  tcase_add_test(tc, sscanf_075_d_trailing_spaces);
  tcase_add_test(tc, sscanf_076_d_leading_newline);
  tcase_add_test(tc, sscanf_077_d_plus_width);
  tcase_add_test(tc, sscanf_078_i_no_prefix_hex_letters_should_fail);
  tcase_add_test(tc, sscanf_079_i_octal_invalid_digit_stops);
  tcase_add_test(tc, sscanf_080_u_leading_spaces);
  tcase_add_test(tc, sscanf_081_o_leading_zero);
  tcase_add_test(tc, sscanf_082_x_mixed_case);
  tcase_add_test(tc, sscanf_083_x_negative_sign);
  tcase_add_test(tc, sscanf_084_p_zero);
  tcase_add_test(tc, sscanf_085_s_stops_at_newline);
  tcase_add_test(tc, sscanf_086_s_leading_tabs);
  tcase_add_test(tc, sscanf_087_s_width_with_space_after);
  tcase_add_test(tc, sscanf_088_c_newline_char);
  tcase_add_test(tc, sscanf_089_space_before_c_skips_whitespace);
  tcase_add_test(tc, sscanf_090_scanset_until_space);
  tcase_add_test(tc, sscanf_091_scanset_invert_a_reads_spaces);
  tcase_add_test(tc, sscanf_092_percent_only_match_no_args);
  tcase_add_test(tc, sscanf_093_percent_only_mismatch_no_args);
  tcase_add_test(tc, sscanf_094_suppress_double_then_int);
  tcase_add_test(tc, sscanf_095_two_ints_first_width);
  tcase_add_test(tc, sscanf_096_mix_literal_comma_space);
  tcase_add_test(tc, sscanf_097_n_after_suppressed_string);
  tcase_add_test(tc, sscanf_098_n_only);
  tcase_add_test(tc, sscanf_099_float_leading_dot);
  tcase_add_test(tc, sscanf_100_float_trailing_dot);

  suite_add_tcase(s, tc);
  return s;
}