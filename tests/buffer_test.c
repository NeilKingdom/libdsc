#include <stdio.h>
#include <stdlib.h>
#include <check.h>
#include "buffer.h"

#define BUF_TYPE int
#define TSIZE sizeof(BUF_TYPE)
#define BLEN (size_t)10

static buffer_t buf; // Make static so that we can reuse it in multiple tests

START_TEST(CreateBuffer)
{
   buf = create_buffer(BLEN, TSIZE);
   ck_assert(buf); // Ensure buffer struct is not NULL
   ck_assert(DSC_BUF_INT(buf)); // Ensure internal buffer memory is not NULL
   ck_assert_int_eq(buffer_len(buf), BLEN); // Ensure length is correct
   ck_assert_int_eq(buffer_size(buf), (BLEN * TSIZE)); // Ensure size is correct
}
END_TEST

START_TEST(ResizeBuffer)
{
   resize_buffer(20, buf);
   ck_assert(DSC_BUF_INT(buf)); // Ensure internal buffer memory is still not NULL
   ck_assert_int_eq(buffer_len(buf), 20); // Ensure length is correct
   ck_assert_int_eq(buffer_size(buf), (20 * TSIZE)); // Ensure size is correct
}
END_TEST

START_TEST(Memcpy)
{
   const char *test_msg = "Hello World!";
   memcpy(DSC_BUF_VOID(buf), (void *)test_msg, strlen(test_msg) + 1);
   ck_assert_str_eq(test_msg, STD_BUF_CHAR(buf)); // Ensure buffer copied string contents properly
}
END_TEST

START_TEST(FreeBuffer)
{
   free_buffer(buf);
   ck_assert(!DSC_BUF_VOID(buf)); // Assert that buf points to NULL
   ck_assert_int_eq(buffer_len(buf), 0); // Assert length is 0
   ck_assert_int_eq(buffer_size(buf), 0); // Assert size is 0
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
