#include <stdio.h>
#include <stdlib.h>
#include <check.h>

#include "stack.h"

START_TEST(CreateStack) {
    Stack_t stack = { 0 };
    long data = 9999999999999;
    dsc_stack_init(&stack, &data, sizeof(long));
    ck_assert_ptr_nonnull(stack.base);
    ck_assert_int_eq(stack.tsize, sizeof(long));
    ck_assert_int_eq(stack.bsize, sizeof(long));
    ck_assert_int_eq(dsc_stack_nelem(&stack), 1);
}
END_TEST

START_TEST(PopStack) {
    Stack_t stack = { 0 };
    dsc_stack_init(&stack, (void*)strdup("Some"), sizeof(char*));
    dsc_stack_push(&stack, (void*)strdup("test"));
    dsc_stack_push(&stack, (void*)strdup("data"));
    ck_assert_int_eq(dsc_stack_nelem(&stack), 3);

    void *top = dsc_stack_peek(&stack);
    ck_assert_str_eq((char*)top, "data");

    dsc_stack_pop(&stack);
    top = dsc_stack_peek(&stack);
    ck_assert_str_eq((char*)top, "test");
}
END_TEST

Suite *buffer_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Stack");

    /* Core test cases */
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, CreateStack);
    tcase_add_test(tc_core, PopStack);
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
