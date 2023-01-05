#include <stdio.h>
#include <stdlib.h>
#include <check.h>

#include "display.h"

START_TEST(CreateBuffer)
{
   size_t blen = 256;
   size_t tsize = sizeof(int);
   buffer_t buf;

   buf.addr = create_buffer(blen, tsize);
   buf.tsize = tsize;

   printf("The length of buffer buf is: %ld", buffer_len(buf));
   assert(buffer_len(buf) == 256);

   /* TODO: Put in separate tests */
   /*resize_buffer(blen, 20, *buf);*/
   /*free_buffer(buffer_len(buf), buf);*/
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
