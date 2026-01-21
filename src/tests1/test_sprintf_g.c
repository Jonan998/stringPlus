#include "tests.h"

START_TEST(sprintf_1_g) {
  char str1[100] = {'\0'};
  char str2[100] = {'\0'};
  char *str3 = "%g Test %5g Test %.g";
  double num = 123.55687;
  ck_assert_int_eq(sprintf(str1, str3, num, num, num),
                   s21_sprintf(str2, str3, num, num, num));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_2_g) {
  char str1[100] = {'\0'};
  char str2[100] = {'\0'};
  char *str3 = "%g\n%.g\n%4g\n%4.g\n%5.10g!";
  double num = -764231539.;
  ck_assert_int_eq(sprintf(str1, str3, num, num, num, num, num),
                   s21_sprintf(str2, str3, num, num, num, num, num));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_3_g) {
  char str1[100] = {'\0'};
  char str2[100] = {'\0'};
  char *str3 = "%#g\n%#.g\n%#5.g\n%#.0g\n%#0.0g!";
  double num = 665695342471359.1;
  ck_assert_int_eq(sprintf(str1, str3, num, num, num, num, num),
                   s21_sprintf(str2, str3, num, num, num, num, num));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_4_g) {
  char str1[300] = {'\0'};
  char str2[300] = {'\0'};
  char *str3 = "New test: %020g\nSecond test: %020.g!";
  double num = -5921563224775967.;
  ck_assert_int_eq(sprintf(str1, str3, num, num),
                   s21_sprintf(str2, str3, num, num));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_5_g) {
  char str1[300] = {'\0'};
  char str2[300] = {'\0'};
  char *str3 = "Third test: %010g\nForth test: %010.g!";
  double num = -5921563224775967.;
  ck_assert_int_eq(sprintf(str1, str3, num, num),
                   s21_sprintf(str2, str3, num, num));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_6_g) {
  char str1[300] = {'\0'};
  char str2[300] = {'\0'};
  char *str3 = "Small: %g, Very small: %.2g";
  double num1 = 0.000000123;
  double num2 = 0.0000123;
  ck_assert_int_eq(sprintf(str1, str3, num1, num2),
                   s21_sprintf(str2, str3, num1, num2));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_7_g) {
  char str1[300] = {'\0'};
  char str2[300] = {'\0'};
  char *str3 = "New test: %020G\nSecond test: %020.G!";
  double num = -5.92156322477597e+15;
  ck_assert_int_eq(sprintf(str1, str3, num, num),
                   s21_sprintf(str2, str3, num, num));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_8_g) {
  char str1[300] = {'\0'};
  char str2[300] = {'\0'};
  char *str3 = "Zeros: %g %g %g %g";
  double n1 = 1.200000;
  double n2 = 1.230000;
  double n3 = 1.000000;
  double n4 = 0.000000;
  ck_assert_int_eq(sprintf(str1, str3, n1, n2, n3, n4),
                   s21_sprintf(str2, str3, n1, n2, n3, n4));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_9_g) {
  char str1[300] = {'\0'};
  char str2[300] = {'\0'};
  char *str3 = "New test: %020g\nSecond test: %020.5g!";
  double num = -592156.322477596;
  ck_assert_int_eq(sprintf(str1, str3, num, num),
                   s21_sprintf(str2, str3, num, num));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_10_g) {
  char str1[300] = {'\0'};
  char str2[300] = {'\0'};
  char *str3 = "Third test: %020.4G\nForth test: %020.3G!";
  double num = -592156.322477596;
  ck_assert_int_eq(sprintf(str1, str3, num, num),
                   s21_sprintf(str2, str3, num, num));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_11_g) {
  char str1[300] = {'\0'};
  char str2[300] = {'\0'};
  char *str3 = "Fifth test: %020.2g\nSixth test: %020.1g!";
  double num = -592156.322477596;
  ck_assert_int_eq(sprintf(str1, str3, num, num),
                   s21_sprintf(str2, str3, num, num));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_12_g) {
  char str1[300] = {'\0'};
  char str2[300] = {'\0'};
  char *str3 = "Seventh test: %020.g\n Eighth test: %020.0g!";
  double num = -592156.322477596;
  ck_assert_int_eq(sprintf(str1, str3, num, num),
                   s21_sprintf(str2, str3, num, num));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_13_g) {
  char str1[300] = {'\0'};
  char str2[300] = {'\0'};
  char *str3 = "New test: % 20g\nSecond test: % 20.5g!";
  double num = -592156.322477596;
  ck_assert_int_eq(sprintf(str1, str3, num, num),
                   s21_sprintf(str2, str3, num, num));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_14_g) {
  char str1[300] = {'\0'};
  char str2[300] = {'\0'};
  char *str3 = "Third test: % 20.4g\nForth test: % 20.3g!";
  double num = -592156.322477596;
  ck_assert_int_eq(sprintf(str1, str3, num, num),
                   s21_sprintf(str2, str3, num, num));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_15_g) {
  char str1[300] = {'\0'};
  char str2[300] = {'\0'};
  char *str3 = "Fifth test: % 20.2g\nSixth test: % 20.1g!";
  double num = -592156.322477596;
  ck_assert_int_eq(sprintf(str1, str3, num, num),
                   s21_sprintf(str2, str3, num, num));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_16_g) {
  char str1[300] = {'\0'};
  char str2[300] = {'\0'};
  char *str3 = "Seventh test: % 20.g\n Eighth test: % 20.0g!";
  double num = -592156.322477596;
  ck_assert_int_eq(sprintf(str1, str3, num, num),
                   s21_sprintf(str2, str3, num, num));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_17_g) {
  char str1[300] = {'\0'};
  char str2[300] = {'\0'};
  char *str3 = "New test: % 31g\nSecond test: % 30.5g!";
  double num = -592156.322477596;
  ck_assert_int_eq(sprintf(str1, str3, num, num),
                   s21_sprintf(str2, str3, num, num));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_18_g) {
  char str1[300] = {'\0'};
  char str2[300] = {'\0'};
  char *str3 = "Third test: % 33.4g\nForth test: % 35.3g!";
  double num = -592156.322477596;
  ck_assert_int_eq(sprintf(str1, str3, num, num),
                   s21_sprintf(str2, str3, num, num));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_19_g) {
  char str1[300] = {'\0'};
  char str2[300] = {'\0'};
  char *str3 = "Fifth test: % 36.2g\nSixth test: % 45.1g!";
  double num = -592156.322477596;
  ck_assert_int_eq(sprintf(str1, str3, num, num),
                   s21_sprintf(str2, str3, num, num));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_20_g) {
  char str1[300] = {'\0'};
  char str2[300] = {'\0'};
  char *str3 = "Seventh test: % 64.g\n Eighth test: % 80.0g!";
  double num = -592156.322477596;
  ck_assert_int_eq(sprintf(str1, str3, num, num),
                   s21_sprintf(str2, str3, num, num));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_21_g) {
  char str1[300] = {'\0'};
  char str2[300] = {'\0'};
  double num1 = -89435776876.;
  double num2 = -894357768.76;
  char *str3 = "New test: %g\nSecond test: %g!";
  ck_assert_int_eq(sprintf(str1, str3, num1, num2),
                   s21_sprintf(str2, str3, num1, num2));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_22_g) {
  char str1[300] = {'\0'};
  char str2[300] = {'\0'};
  double num3 = -89435.776876;
  double num4 = -0.89435776876;
  char *str3 = "Third test: %g\nForth test: %g\n!";
  ck_assert_int_eq(sprintf(str1, str3, num3, num4),
                   s21_sprintf(str2, str3, num3, num4));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_23_g) {
  char str1[300] = {'\0'};
  char str2[300] = {'\0'};
  double num1 = -.89435776876;
  double num2 = -.00089435776876;
  char *str3 = "New test: %g\nSecond test: %g!";
  ck_assert_int_eq(sprintf(str1, str3, num1, num2),
                   s21_sprintf(str2, str3, num1, num2));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_24_g) {
  char str1[300] = {'\0'};
  char str2[300] = {'\0'};
  double num3 = -0.000000089435776876;
  double num4 = -0000000000000.89435776876;
  char *str3 = "Third test: %g\nForth test: %g\n!";
  ck_assert_int_eq(sprintf(str1, str3, num3, num4),
                   s21_sprintf(str2, str3, num3, num4));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_25_g) {
  char str1[300] = {'\0'};
  char str2[300] = {'\0'};
  double num1 = -.89435776876;
  double num2 = -.00089435776876;
  double num3 = -0.000000089435776876;
  double num4 = -0000000000000.89435776876;
  char *str3 = "test: %.G!\ntest: %#G!\ntest: %#.G!\ntest: %0.G!\n";
  ck_assert_int_eq(sprintf(str1, str3, num1, num2, num3, num4),
                   s21_sprintf(str2, str3, num1, num2, num3, num4));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_26_g) {
  char str1[300] = {'\0'};
  char str2[300] = {'\0'};
  double num1 = -.3529373967;
  double num2 = -.0003529373967;
  double num3 = -0.00000003529373967;
  char *str3 = "test: % 013.G\ntest: %#4.9G\ntest: %016.2G!";
  ck_assert_int_eq(sprintf(str1, str3, num1, num2, num3),
                   s21_sprintf(str2, str3, num1, num2, num3));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_27_g) {
  char str1[300] = {'\0'};
  char str2[300] = {'\0'};
  double num1 = -.3529373967;
  double num4 = -0000000000000.3529373967;
  char *str3 = "test: %20.g\ntest: %-20.g\n!";
  ck_assert_int_eq(sprintf(str1, str3, num4, num1),
                   s21_sprintf(str2, str3, num4, num1));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_28_g) {
  char str1[300] = {'\0'};
  char str2[300] = {'\0'};
  double num1 = .3529373967;
  double num2 = .0003529373967;
  double num3 = 0.00000003529373967;
  char *str3 = "test: % 30.1G\ntest: % 30.g\ntest: %030.0G!";
  ck_assert_int_eq(sprintf(str1, str3, num1, num2, num3),
                   s21_sprintf(str2, str3, num1, num2, num3));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_29_g) {
  char str1[300] = {'\0'};
  char str2[300] = {'\0'};
  double num1 = .3529373967;
  double num4 = 0000000000000.3529373967;
  char *str3 = "test: %066.g\ntest: %-20.g\n!";
  ck_assert_int_eq(sprintf(str1, str3, num4, num1),
                   s21_sprintf(str2, str3, num4, num1));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_30_g) {
  char str1[300] = {'\0'};
  char str2[300] = {'\0'};
  double num1 = 53296575676899283734747273752737878257.;
  char *str3 = "test: %- 17.1G\ntest: % -17.1G!";
  ck_assert_int_eq(sprintf(str1, str3, num1, num1),
                   s21_sprintf(str2, str3, num1, num1));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_31_g) {
  char str1[300] = {'\0'};
  char str2[300] = {'\0'};
  double num2 = -53296575676899283734747273752737878257.;
  char *str3 = "test: %# 10.2G\ntest: % #10.0G\ntest: %010.g\n!";
  ck_assert_int_eq(sprintf(str1, str3, num2, num2, num2),
                   s21_sprintf(str2, str3, num2, num2, num2));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_32_g) {
  char str1[600] = {'\0'};
  char str2[600] = {'\0'};
  double num1 = 1234567890000.0938217586;
  char *str3 = "test: %- 67.1G\ntest: % -67.1G!";
  ck_assert_int_eq(sprintf(str1, str3, num1, num1),
                   s21_sprintf(str2, str3, num1, num1));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_33_g) {
  char str1[600] = {'\0'};
  char str2[600] = {'\0'};
  double num2 = -.12345678900000938217586;
  char *str3 = "test: %# 100.0G\ntest: % #100.0G\ntest: %0100.g\n!";
  ck_assert_int_eq(sprintf(str1, str3, num2, num2, num2),
                   s21_sprintf(str2, str3, num2, num2, num2));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_34_g) {
  char str1[1000] = {'\0'};
  char str2[1000] = {'\0'};
  double num1 = 1234567890000.0938217586;
  double num2 = -.12345678900000938217586;
  char *str3 = "test: % 070.10G\ntest: % -67.10G\n!";
  ck_assert_int_eq(sprintf(str1, str3, num1, num2),
                   s21_sprintf(str2, str3, num1, num2));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_35_g) {
  char str1[1000] = {'\0'};
  char str2[1000] = {'\0'};
  double num1 = -12345678900000938217586.;
  double num2 = .12345678900000938217586;
  char *str3 = "test: % 070.10G\ntest: % -67.10G\n!";
  ck_assert_int_eq(sprintf(str1, str3, num1, num2),
                   s21_sprintf(str2, str3, num1, num2));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_36_g) {
  char str1[600] = {'\0'};
  char str2[600] = {'\0'};
  double num1 = 53296575676899283734747273752737878257.;
  char *str3 = "test: % 10.1G\ntest: %- 10.1G!";
  ck_assert_int_eq(sprintf(str1, str3, num1, num1),
                   s21_sprintf(str2, str3, num1, num1));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_37_g) {
  char str1[600] = {'\0'};
  char str2[600] = {'\0'};
  double num2 = 53296575676899283.734747273752737878257;
  char *str3 = "test: % -10.0G\ntest: %- 10.0G\ntest: % -10.g\n!";
  ck_assert_int_eq(sprintf(str1, str3, num2, num2, num2),
                   s21_sprintf(str2, str3, num2, num2, num2));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_38_g) {
  char str1[600] = {'\0'};
  char str2[600] = {'\0'};
  double num1 = .000000005329657567689928373474727375273787825732159676631957;
  double num2 = 53296575676899283.73474727375273787825732159676631957;
  char *str3 = "test: % 20g\ntest: %-20g\ntest: %+ 020.g\n!";
  ck_assert_int_eq(sprintf(str1, str3, num1, num2, num1),
                   s21_sprintf(str2, str3, num1, num2, num1));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_39_g) {
  char str1[600] = {'\0'};
  char str2[600] = {'\0'};
  double num3 = 5329657567689928373474727375.273787825732159676631957;
  double num4 = 53296575676899283734747273752737878257.32159676631957;
  char *str3 = "test: %#20.g\ntest: %- 20g!";
  ck_assert_int_eq(sprintf(str1, str3, num3, num4),
                   s21_sprintf(str2, str3, num3, num4));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_40_g) {
  char str1[100] = {'\0'};
  char str2[100] = {'\0'};
  char *str3 = "%.g\n%.2g\n%.5G\n%.9G\n%.13G!";
  double num = -2599999999.;
  ck_assert_int_eq(sprintf(str1, str3, num, num, num, num, num),
                   s21_sprintf(str2, str3, num, num, num, num, num));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_41_g) {
  char str1[200] = {'\0'};
  char str2[200] = {'\0'};
  char *str3 = "%.Lg\n%.2Lg\n%.5Lg\n%.9Lg\n%.13LG!";
  long double num = -2599999999.;
  ck_assert_int_eq(sprintf(str1, str3, num, num, num, num, num),
                   s21_sprintf(str2, str3, num, num, num, num, num));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_42_g) {
  char str1[200] = {'\0'};
  char str2[200] = {'\0'};
  char *str3 = "%g\n%g\n%g\n%g\n%G!";
  double num =
      93257812350000000000000000000683689988320616583292392563908359623906832590653989623598263628236642932982362398.;
  ck_assert_int_eq(sprintf(str1, str3, num, num, num, num, num),
                   s21_sprintf(str2, str3, num, num, num, num, num));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_43_g) {
  char str1[400] = {'\0'};
  char str2[400] = {'\0'};
  char *str3 = "test: %20Lg!\ntest: %Lg!\ntest: %-10Lg!";
  long double num =
      -93257812350000000000000000000683689988320616583292392563908359623906832590653989623598263628236642932982362398.;
  ck_assert_int_eq(sprintf(str1, str3, num, num, num),
                   s21_sprintf(str2, str3, num, num, num));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_44_g) {
  char str1[400] = {'\0'};
  char str2[400] = {'\0'};
  char *str3 = "test: %060Lg!\ntest: %.14Lg!";
  long double num =
      -93257812350000000000000000000683689988320616583292392563908359623906832590653989623598263628236642932982362398.;
  ck_assert_int_eq(sprintf(str1, str3, num, num),
                   s21_sprintf(str2, str3, num, num));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

Suite *test_sprintf_g(void) {
  Suite *s = suite_create("\033[45m-=S21_SPRINTF_G=-\033[0m");
  TCase *tc = tcase_create("sprintf_tc");

  tcase_add_test(tc, sprintf_1_g);
  tcase_add_test(tc, sprintf_2_g);
  tcase_add_test(tc, sprintf_3_g);
  tcase_add_test(tc, sprintf_4_g);
  tcase_add_test(tc, sprintf_5_g);
  tcase_add_test(tc, sprintf_6_g);
  tcase_add_test(tc, sprintf_7_g);
  tcase_add_test(tc, sprintf_8_g);
  tcase_add_test(tc, sprintf_9_g);
  tcase_add_test(tc, sprintf_10_g);
  tcase_add_test(tc, sprintf_11_g);
  tcase_add_test(tc, sprintf_12_g);
  tcase_add_test(tc, sprintf_13_g);
  tcase_add_test(tc, sprintf_14_g);
  tcase_add_test(tc, sprintf_15_g);
  tcase_add_test(tc, sprintf_16_g);
  tcase_add_test(tc, sprintf_17_g);
  tcase_add_test(tc, sprintf_18_g);
  tcase_add_test(tc, sprintf_19_g);
  tcase_add_test(tc, sprintf_20_g);
  tcase_add_test(tc, sprintf_21_g);
  tcase_add_test(tc, sprintf_22_g);
  tcase_add_test(tc, sprintf_23_g);
  tcase_add_test(tc, sprintf_24_g);
  tcase_add_test(tc, sprintf_25_g);
  tcase_add_test(tc, sprintf_26_g);
  tcase_add_test(tc, sprintf_27_g);
  tcase_add_test(tc, sprintf_28_g);
  tcase_add_test(tc, sprintf_29_g);
  tcase_add_test(tc, sprintf_30_g);
  tcase_add_test(tc, sprintf_31_g);
  tcase_add_test(tc, sprintf_32_g);
  tcase_add_test(tc, sprintf_33_g);
  tcase_add_test(tc, sprintf_34_g);
  tcase_add_test(tc, sprintf_35_g);
  tcase_add_test(tc, sprintf_36_g);
  tcase_add_test(tc, sprintf_37_g);
  tcase_add_test(tc, sprintf_38_g);
  tcase_add_test(tc, sprintf_39_g);
  tcase_add_test(tc, sprintf_40_g);
  tcase_add_test(tc, sprintf_41_g);
  tcase_add_test(tc, sprintf_42_g);
  tcase_add_test(tc, sprintf_43_g);
  tcase_add_test(tc, sprintf_44_g);

  suite_add_tcase(s, tc);
  return s;
}