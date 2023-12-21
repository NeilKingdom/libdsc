#include <stdio.h>
#include <stdlib.h>
#include <check.h>

#include "../include/buffer.h"

#define BTYPE int
#define TSIZE sizeof(BTYPE)
#define BLEN (size_t)10

START_TEST(CreateBuffer) {
    buffer_t buf = dsc_create_buffer(BLEN, TSIZE);
    ck_assert_ptr_nonnull(buf.addr); // Ensure internal buffer memory is not NULL
    ck_assert_int_eq((int)dsc_get_buffer_capacity(buf), BLEN); // Ensure length is correct
    ck_assert_int_eq((int)buf.bsize, (BLEN * TSIZE)); // Ensure size is correct
}
END_TEST

START_TEST(ResizeBuffer) {
    buffer_t buf = dsc_create_buffer(BLEN, TSIZE);

#undef BLEN
#define BLEN (size_t)20

    dsc_resize_buffer(&buf, BLEN);
    ck_assert_ptr_nonnull(buf.addr); // Ensure internal buffer memory is still not NULL
    ck_assert_int_eq((int)dsc_get_buffer_capacity(buf), BLEN); // Ensure length is correct
    ck_assert_int_eq((int)buf.bsize, (BLEN * TSIZE)); // Ensure size is correct
    dsc_destroy_buffer(&buf);
}
END_TEST

START_TEST(Memcpy) {
    const char *test_msg = "Hello World!";
    buffer_t buf = dsc_create_buffer(BLEN, TSIZE);
    memcpy(DSC_BUF_VOID(buf), (void *)test_msg, strlen(test_msg) + 1);
    ck_assert_str_eq(test_msg, DSC_BUF_CHAR(buf)); // Ensure buffer copied string contents properly
    dsc_destroy_buffer(&buf);
}
END_TEST

START_TEST(FreeBuffer) {
    buffer_t buf = dsc_create_buffer(BLEN, TSIZE);
    dsc_destroy_buffer(&buf);
    ck_assert_ptr_null(buf.addr); // Assert that buf is not a dangling pointer
    ck_assert_int_eq((int)buf.bsize, 0); // Assert size is 0
    ck_assert_int_eq(dsc_destroy_buffer(&buf), DSC_EFREE); // Double free
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
