#include <stdio.h>
#include <stdlib.h>
#include <check.h>

#include "buffer.h"

START_TEST(CreateBuffer) {
    Buffer_t buf = dsc_buf_create(10, sizeof(int));
    ck_assert_ptr_nonnull(buf.addr);
    ck_assert_int_eq(buf.tsize, sizeof(int));
    ck_assert_int_eq(dsc_buf_capacity(buf), (10 * sizeof(int)));
    dsc_buf_destroy(&buf);
}
END_TEST

START_TEST(ResizeBuffer) {
    Buffer_t buf = dsc_buf_create(5, sizeof(short));
    dsc_buf_resize(&buf, 10);
    ck_assert_int_eq(dsc_buf_capacity(buf), (10 * sizeof(short)));
    ck_assert_int_eq(buf.tsize, sizeof(short));
    dsc_buf_destroy(&buf);
}
END_TEST

START_TEST(Memcpy) {
    const char *test_str = "Hello, World!";
    Buffer_t buf = dsc_buf_create(1, sizeof(char*));
    memcpy(buf.addr, test_str, strlen(test_str) + 1);
    ck_assert_str_eq(test_str, DSC_BUF_AS_CHAR(buf));
    dsc_buf_destroy(&buf);
}
END_TEST

START_TEST(FreeBuffer) {
    Buffer_t buf = dsc_buf_create(1, 1);
    dsc_buf_destroy(&buf);
    ck_assert_ptr_null(buf.addr);
    ck_assert_int_eq(buf.bsize, 0);
    ck_assert_int_eq(dsc_buf_destroy(&buf), DSC_EFAIL);
}
END_TEST

Suite *buffer_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Buffer");

    /* Core test cases */
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, CreateBuffer);
    tcase_add_test(tc_core, ResizeBuffer);
    tcase_add_test(tc_core, Memcpy);
    tcase_add_test(tc_core, FreeBuffer);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void) {
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
