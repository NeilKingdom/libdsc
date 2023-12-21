#include <stdio.h>
#include <stdlib.h>
#include <check.h>

#include "../include/stack.h"

#define STYPE short
#define TSIZE sizeof(STYPE)

START_TEST(CreateStack) {
    stack_t stack = dsc_create_stack(TSIZE);
    ck_assert_ptr_nonnull(stack.arena.addr); // Ensure internal buffer memory is not NULL
    ck_assert_int_eq((int)dsc_get_stack_size(&stack), 0); // Stack size should be 0
    ck_assert_int_eq((int)stack.offset, 0); // Ensure offset is 0
    dsc_destroy_stack(&stack);
}
END_TEST

START_TEST(PushStack) {
    short data = 16000;
    stack_t stack = dsc_create_stack(TSIZE);
    dsc_push_stack(&stack, (void*)&data);
    ck_assert_ptr_nonnull(dsc_peek_stack(&stack)); // Ensure peek returned real data
    ck_assert_int_eq(*((short*)(dsc_peek_stack(&stack))), data); // Peek should return the data on top of stack
    ck_assert_int_eq((int)dsc_get_stack_size(&stack), 1); // Stack size should be 1
    ck_assert_int_eq((int)stack.offset, TSIZE); // Offset should be incremented by a factor of TSIZE
    dsc_destroy_stack(&stack);
}
END_TEST

START_TEST(PopStack) {
    short data_vec[] = { 3, 2, 1 };
    size_t i, dv_size = sizeof(data_vec) / sizeof(*data_vec);
    stack_t stack = dsc_create_stack(TSIZE);
    // Push items
    for (i = 0; i < dv_size; ++i) {
        dsc_push_stack(&stack, (void*)(data_vec + i));
    }
    // Pop order should be 1, 2, 3
    for (i = 0; i < dv_size; ++i) {
        ck_assert_int_eq(*((short*)(dsc_pop_stack(&stack))), data_vec[dv_size - i - 1]);
    }
    ck_assert_ptr_null(dsc_pop_stack(&stack)); // Pop when empty returns NULL
    dsc_destroy_stack(&stack);
}
END_TEST

START_TEST(DestroyStack) {
    stack_t stack = dsc_create_stack(TSIZE);
    dsc_destroy_stack(&stack);
    ck_assert_ptr_null(stack.arena.addr); // Assert that buf is not a dangling pointer
    ck_assert_int_eq((int)stack.arena.bsize, 0); // Assert size is 0
    ck_assert_int_eq((int)stack.offset, 0); // Offset should be 0
    ck_assert_int_eq(dsc_destroy_stack(&stack), DSC_EFREE); // Double free
}
END_TEST

Suite *stack_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Stack");

    /* Core test cases */
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, CreateStack);
    tcase_add_test(tc_core, PushStack);
    tcase_add_test(tc_core, PopStack);
    tcase_add_test(tc_core, DestroyStack);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void) {
    int num_failed;
    Suite *s;
    SRunner *sr;

    s = stack_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    num_failed = srunner_ntests_failed(sr);
    printf("%s\n", num_failed ? "At least one test failed" : "All tests passed");
    srunner_free(sr);
    return (!num_failed ? EXIT_SUCCESS : EXIT_FAILURE);
}
