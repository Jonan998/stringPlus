#include <check.h>
#include <stdlib.h>
#include <string.h>

#include "../s21_string.h"

START_TEST(test_strlen_basic) {
  ck_assert_int_eq(s21_strlen("hello"), strlen("hello"));
}
END_TEST

START_TEST(test_strlen_empty) { ck_assert_int_eq(s21_strlen(""), strlen("")); }
END_TEST

START_TEST(test_memcpy_basic) {
  char s1[10] = "abcdef";
  char s2[10] = "abcdef";
  s21_memcpy(s1, "123", 3);
  memcpy(s2, "123", 3);
  ck_assert_mem_eq(s1, s2, 6);
}
END_TEST

START_TEST(test_memset_basic) {
  char s1[10] = "abcdef";
  char s2[10] = "abcdef";
  s21_memset(s1, 'x', 3);
  memset(s2, 'x', 3);
  ck_assert_mem_eq(s1, s2, 6);
}
END_TEST

START_TEST(test_memchr_basic) {
  char s[] = "abcdef";
  ck_assert_ptr_eq(s21_memchr(s, 'c', 6), memchr(s, 'c', 6));
}
END_TEST

START_TEST(test_strncmp_equal) {
  ck_assert_int_eq(s21_strncmp("hello", "hello", 5),
                   strncmp("hello", "hello", 5));
}
END_TEST

START_TEST(test_strncmp_diff) {
  ck_assert_int_eq(s21_strncmp("abc", "abd", 3), strncmp("abc", "abd", 3));
}
END_TEST

START_TEST(test_memcmp_basic) {
  ck_assert_int_eq(s21_memcmp("abc", "abd", 3), memcmp("abc", "abd", 3));
}
END_TEST

START_TEST(test_strcspn_basic) {
  ck_assert_int_eq(s21_strcspn("hello", "l"), strcspn("hello", "l"));
}
END_TEST

START_TEST(test_strcat_basic) {
  char s1[20] = "hi ";
  char s2[20] = "hi ";
  ck_assert_str_eq(s21_strcat(s1, "there"), strcat(s2, "there"));
}
END_TEST

START_TEST(test_strncpy_basic) {
  char a[10], b[10];
  s21_strncpy(a, "hi", 5);
  strncpy(b, "hi", 5);
  ck_assert_mem_eq(a, b, 5);
}
END_TEST

START_TEST(test_strpbrk_basic) {
  ck_assert_ptr_eq(s21_strpbrk("hello", "xyzl"), strpbrk("hello", "xyzl"));
}
END_TEST

START_TEST(test_strrchr_basic) {
  ck_assert_ptr_eq(s21_strrchr("hello", 'l'), strrchr("hello", 'l'));
}
END_TEST

START_TEST(test_strerror_basic) {
  ck_assert_str_eq(s21_strerror(1), strerror(1));
}
END_TEST

START_TEST(test_strchr_found) {
  ck_assert_ptr_eq(s21_strchr("hello", 'e'), strchr("hello", 'e'));
}
END_TEST

START_TEST(test_strchr_null) {
  ck_assert_ptr_eq(s21_strchr("hello", 'x'), strchr("hello", 'x'));
}
END_TEST

START_TEST(test_strstr_basic) {
  ck_assert_ptr_eq(s21_strstr("hello world", "world"),
                   strstr("hello world", "world"));
}
END_TEST

START_TEST(test_strstr_empty) {
  ck_assert_ptr_eq(s21_strstr("hello", ""), strstr("hello", ""));
}
END_TEST

START_TEST(test_strtok_basic) {
  char s1[] = "a,b,c";
  char s2[] = "a,b,c";

  char *t1 = s21_strtok(s1, ",");
  char *t2 = strtok(s2, ",");

  while (t1 || t2) {
    ck_assert_str_eq(t1, t2);
    t1 = s21_strtok(NULL, ",");
    t2 = strtok(NULL, ",");
  }
}
END_TEST

START_TEST(test_to_upper) {
  char *res = s21_to_upper("Abc");
  ck_assert_str_eq(res, "ABC");
  free(res);
}
END_TEST

START_TEST(test_to_lower) {
  char *res = s21_to_lower("AbC");
  ck_assert_str_eq(res, "abc");
  free(res);
}
END_TEST

START_TEST(test_insert) {
  char *res = s21_insert("Hello", " world", 5);
  ck_assert_str_eq(res, "Hello world");
  free(res);
}
END_TEST

START_TEST(test_trim) {
  char *res = s21_trim("  hello  ", " ");
  ck_assert_str_eq(res, "hello");
  free(res);
}
END_TEST

Suite *s21_string_suite(void) {
  Suite *s = suite_create("s21_string");

  TCase *tc = tcase_create("core");
  tcase_add_test(tc, test_strlen_basic);
  tcase_add_test(tc, test_strlen_empty);
  tcase_add_test(tc, test_memcpy_basic);
  tcase_add_test(tc, test_memset_basic);
  tcase_add_test(tc, test_strncmp_equal);
  tcase_add_test(tc, test_strncmp_diff);
  tcase_add_test(tc, test_strcat_basic);
  tcase_add_test(tc, test_strchr_found);
  tcase_add_test(tc, test_strchr_null);
  tcase_add_test(tc, test_strstr_basic);
  tcase_add_test(tc, test_strstr_empty);
  tcase_add_test(tc, test_strtok_basic);
  tcase_add_test(tc, test_to_upper);
  tcase_add_test(tc, test_to_lower);
  tcase_add_test(tc, test_insert);
  tcase_add_test(tc, test_trim);
  tcase_add_test(tc, test_memchr_basic);
  tcase_add_test(tc, test_memcmp_basic);
  tcase_add_test(tc, test_strcspn_basic);
  tcase_add_test(tc, test_strncpy_basic);
  tcase_add_test(tc, test_strpbrk_basic);
  tcase_add_test(tc, test_strrchr_basic);
  tcase_add_test(tc, test_strerror_basic);

  suite_add_tcase(s, tc);
  return s;
}

int main(void) {
  Suite *s = s21_string_suite();
  SRunner *sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  int failed = srunner_ntests_failed(sr);

  srunner_free(sr);
  return failed == 0 ? 0 : 1;
}
