#include <stdio.h>
#include <stdlib.h>
#include <check.h>

#include "display.h"

START_TEST(CreateBuffer)
{
   int buf_len = 256;

   pBuffer_t ibuf;
   INIT_BUFFER(ibuf);
   ibuf->begin = create_buffer(buf_len, sizeof(int));

   assert(ibuf->type == DSC_BUFFER);
   assert(sizeof_buffer(ibuf->begin) == 256 * sizeof(int));
}
END_TEST

int run_testsuite()
{
   int fail_nr;
   Suite *s;
   SRunner *sr;

   s = test_suite();
   sr = srunner_create(s);

   srunner_run_all(sr, CK_NORMAL);
   fail_nr = srunner_ntests_failed(sr);
   srunner_free(sr);

   printf("%s\n", fail_nr ? "At least one test failed" : "All tests passed");

   return (!fail_nr ? EXIT_SUCCESS : EXIT_FAILURE);
}
