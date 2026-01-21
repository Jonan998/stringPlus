#include "tests.h"

#include <limits.h>

int main() {
  int failed = 0;
  int alls = 0;

  Suite *s21_string_test[] = {test_sprintf_f(),
                              test_sprintf_e(),
                              test_sprintf_g(),
                              test_sprintf_hex(),
                              test_sprintf_HEX(),
                              test_sprintf_c(),
                              test_sprintf_signed(),
                              test_sprintf_octal(),
                              test_sprintf_percent(),
                              test_sprintf_string(),
                              test_sprintf_pointer(),
                              test_sprintf_unsigned(),
                              s21_NULL};

  for (int i = 0; s21_string_test[i] != s21_NULL; i++) {
    SRunner *sr = srunner_create(s21_string_test[i]);

    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_run_all(sr, CK_NORMAL);

    failed += srunner_ntests_failed(sr);
    alls += srunner_ntests_run(sr);
    srunner_free(sr);
  }

  printf("========= FAILED: %d =========\n", failed);
  printf("========= ALLS: %d =========\n", alls);
  return (failed == 0) ? 0 : 1;
}