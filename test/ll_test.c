#include <stdio.h>
#include <stdlib.h>
#include <check.h>

#include "../include/ll.h"
#include "../include/buffer.h"

#define BTYPE char[32]
#define TSIZE sizeof(BTYPE)

START_TEST(CreateLL) {
    const char *greeting = "Hello, World!";
    pLLNode_t head = dsc_create_ll(TSIZE, (void*)greeting);
    ck_assert_int_eq((int)head->data.tsize, (int)TSIZE);
    ck_assert_ptr_null(head->next);
    ck_assert_str_eq(DSC_BUF_CHAR(head->data), greeting);
    dsc_destroy_ll(head);
}
END_TEST

START_TEST(AddNode) {
    int i;
    const char* const list[] = { "Foo", "Bar", "Baz" };
    pLLNode_t head = dsc_create_ll(TSIZE, (void*)list[0]);
    pLLNode_t tmp = head;
    dsc_add_ll_node(head, (void*)list[1]);
    dsc_add_ll_node(head, (void*)list[2]);

    for (i = 0; tmp->next; tmp = tmp->next, ++i) {
        ck_assert_str_eq(DSC_BUF_CHAR(tmp->data), list[i]);
    }

    dsc_destroy_ll(head);
}
END_TEST

START_TEST(DelNode) {
    int i;
    const char* const list[] = { "Foo", "Bar", "Baz" };
    pLLNode_t head = dsc_create_ll(TSIZE, (void*)list[0]);
    pLLNode_t tmp = head;
    dsc_add_ll_node(head, (void*)list[1]);
    dsc_add_ll_node(head, (void*)list[2]);

    ck_assert_int_eq((int)dsc_get_ll_len(head), (int)(sizeof(list) / sizeof(*list))); // List lengths match
    dsc_del_ll_node(head, 1); // Delete item at index 1 (Bar)
    ck_assert_int_eq((int)dsc_get_ll_len(head), (int)((sizeof(list) / sizeof(*list)) - 1)); // List lengths match
    ck_assert_str_eq(DSC_BUF_CHAR(head->data), list[0]); // Head contains "Foo"
    ck_assert_str_eq(DSC_BUF_CHAR(head->next->data), list[2]); // Head->next / tail contains "Baz"

    dsc_destroy_ll(head);
}
END_TEST

START_TEST(GetNode) {
}
END_TEST

Suite *buffer_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Buffer");

    /* Core test cases */
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, CreateLL);
    tcase_add_test(tc_core, AddNode);
    /*tcase_add_test(tc_core, Memcpy);
    tcase_add_test(tc_core, FreeBuffer);*/
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
