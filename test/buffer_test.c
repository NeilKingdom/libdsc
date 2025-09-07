#include <check.h>

#include "dsc_common.h"
#include "buffer.h"

START_TEST(CreateBuffer) {
    Buffer_t buf = { 0 };
    dsc_buf_init(&buf, 10, sizeof(int));

    ck_assert_ptr_nonnull(buf.addr);
    ck_assert_int_eq(buf.tsize, sizeof(int));
    ck_assert_int_eq(buf.bsize, 10 * sizeof(int));
    ck_assert_int_eq(dsc_buf_nelem(&buf), 10);
}
END_TEST

START_TEST(ResizeBuffer) {
    Buffer_t buf = { 0 };

    dsc_buf_init(&buf, 5, sizeof(short));
    ck_assert_int_eq(buf.tsize, sizeof(short));
    ck_assert_int_eq(buf.bsize, 5 * sizeof(short));
    ck_assert_int_eq(dsc_buf_nelem(&buf), 5);

    dsc_buf_resize(&buf, 10);
    ck_assert_int_eq(buf.tsize, sizeof(short));
    ck_assert_int_eq(buf.bsize, 10 * sizeof(short));
    ck_assert_int_eq(dsc_buf_nelem(&buf), 10);
}
END_TEST

START_TEST(Memcpy) {
    const char *test_str = "Hello, World!";
    Buffer_t buf = { 0 };
    dsc_buf_init(&buf, 1, sizeof(char*));
    memcpy(buf.addr, test_str, strlen(test_str) + 1);
    ck_assert_str_eq((char*)buf.addr, test_str);
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
