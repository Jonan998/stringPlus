#include "tests.h"

Suite *test_sscanf(void);

int main(void) {
  SRunner *sr = srunner_create(NULL);

  srunner_add_suite(sr, test_sscanf());

  srunner_run_all(sr, CK_NORMAL);
  int nf = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (nf == 0) ? 0 : 1;
}