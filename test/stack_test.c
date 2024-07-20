#include <stdio.h>
#include <stdlib.h>
#include <check.h>

#include "stack.h"

START_TEST(CreateStack) {
    Stack_t stack = dsc_stack_create(sizeof(long));
    ck_assert_ptr_nonnull(stack.addr);
    ck_assert_int_eq(stack.tsize, sizeof(long));
    ck_assert_int_eq(dsc_stack_capacity(stack), sizeof(int));
    dsc_stack_destroy(&stack);
}
END_TEST

START_TEST(PushStack) {
    Stack_t stack = dsc_stack_create(sizeof(char*));
    dsc_stack_push(&stack, (void*)("Some"));
    dsc_stack_push(&stack, (void*)("test"));
    dsc_stack_push(&stack, (void*)("data"));
    ck_assert_int_eq(dsc_stack_capacity(stack), (3 * sizeof(char*)));

    Buffer_t top = dsc_stack_peek(stack);
    ck_assert_str_eq(DSC_BUF_AS_CHAR(top), "data");
    dsc_buf_destroy(&top);

    /* Repeat to make sure data is still present */
    top = dsc_stack_peek(stack);
    ck_assert_str_eq(DSC_BUF_AS_CHAR(top), "data");
    dsc_buf_destroy(&top);

    dsc_stack_destroy(&stack);
}
END_TEST

START_TEST(PopStack) {
    Stack_t stack = dsc_stack_create(sizeof(char*));
    dsc_stack_push(&stack, (void*)("Some"));
    dsc_stack_push(&stack, (void*)("test"));
    dsc_stack_push(&stack, (void*)("data"));
    ck_assert_int_eq(dsc_stack_capacity(stack), (3 * sizeof(char*)));

    Buffer_t top = dsc_stack_pop(stack);
    ck_assert_str_eq(DSC_BUF_AS_CHAR(top), "data");
    dsc_buf_destroy(&top);

    top = dsc_stack_peek(stack);
    ck_assert_str_eq(DSC_BUF_AS_CHAR(top), "test");
    dsc_buf_destroy(&top);

    dsc_stack_destroy(&stack);
}
END_TEST

START_TEST(PeekStack) {
    ck_assert(true);
}
END_TEST

Suite *buffer_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Stack");

    /* Core test cases */
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, CreateStack);
    tcase_add_test(tc_core, PushStack);
    tcase_add_test(tc_core, PopStack);
    tcase_add_test(tc_core, PeekStack);
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
