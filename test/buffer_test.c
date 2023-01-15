#include <stdio.h>
#include <stdlib.h>
#include <check.h>
#include "buffer.h"

#define BUF_TYPE int
#define TSIZE sizeof(BUF_TYPE)
#define BLEN (size_t)10

static pBuffer_t bp; // Make static so that we can reuse it in multiple tests

START_TEST(CreateBuffer)
{
   bp = create_buffer(BLEN, TSIZE);
   ck_assert(bp); // Ensure buffer struct is not NULL
   ck_assert(DSC_BUF_INT(bp)); // Ensure internal buffer memory is not NULL
   ck_assert_int_eq((int)buffer_len(bp), BLEN); // Ensure length is correct
   ck_assert_int_eq((int)buffer_size(bp), (BLEN * TSIZE)); // Ensure size is correct
}
END_TEST

START_TEST(ResizeBuffer)
{
   resize_buffer(20, bp);
   ck_assert(DSC_BUF_INT(bp)); // Ensure internal buffer memory is still not NULL
   ck_assert_int_eq((int)buffer_len(bp), 20); // Ensure length is correct
   ck_assert_int_eq((int)buffer_size(bp), (20 * TSIZE)); // Ensure size is correct
}
END_TEST

START_TEST(Memcpy)
{
   const char *test_msg = "Hello World!";
   memcpy(DSC_BUF_VOID(bp), (void *)test_msg, strlen(test_msg) + 1);
   ck_assert_str_eq(test_msg, DSC_BUF_CHAR(bp)); // Ensure buffer copied string contents properly
}
END_TEST

START_TEST(FreeBuffer)
{
   free_buffer(bp);
   ck_assert_ptr_null(DSC_BUF_VOID(bp)); // Assert that buf points to NULL
   ck_assert_int_eq((int)buffer_len(bp), 0); // Assert length is 0
   ck_assert_int_eq((int)buffer_size(bp), 0); // Assert size is 0
}
END_TEST

Suite *buffer_suite(void)
{
   Suite *s;
   TCase *tc_core;

   s = suite_create("Buffer");

   /* Core test cases */
   tc_core = tcase_create("Core");
   tcase_add_test(tc_core, CreateBuffer);
   tcase_add_test(tc_core, ResizeBuffer);
   tcase_add_test(tc_core, Memcpy);
   tcase_add_test(tc_core, FreeBuffer);

   return s;
}

int main(void)
{
   int num_failed;
   Suite *s;
   SRunner *sr;

   s = buffer_suite();
   sr = srunner_create(s);

   srunner_run_all(sr, CK_NORMAL);
   num_failed = srunner_ntests_failed(sr);
   printf("%s\n", num_failed ? "At least one test failed" : "All tests passed");
   srunner_free(sr);
   return (!num_failed ? EXIT_SUCCESS : EXIT_FAILURE);
}
