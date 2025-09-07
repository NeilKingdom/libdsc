#include <check.h>

#include "dsc_common.h"
#include "ll.h"

START_TEST(CreateLL) {
    const char *test_str = "Hello, World!";

    LLNode_t *head = dsc_ll_create(sizeof(char*), (void*)test_str);
    ck_assert_int_eq(head->data.tsize, sizeof(char*));
    ck_assert_ptr_null(head->next);
    ck_assert_str_eq(DSC_BUF_AS_CHAR(head->data), test_str);

    dsc_ll_destroy(head);
}
END_TEST

START_TEST(AddNode) {
    int i;
    const char* const list[] = { "Foo", "Bar", "Baz" };

    LLNode_t *head = dsc_ll_create(sizeof(list[0]), (void*)list[0]);
    dsc_ll_add_node(head, (void*)list[1]);
    dsc_ll_add_node(head, (void*)list[2]);

    LLNode_t *tmp = head;
    for (i = 0; tmp->next; tmp = tmp->next, ++i) {
        ck_assert_str_eq(DSC_BUF_AS_CHAR(tmp->data), list[i]);
    }

    dsc_ll_destroy(head);
}
END_TEST

START_TEST(RemoveNode) {
    int i;
    const char* const list[] = { "Foo", "Bar", "Baz" };

    LLNode_t *head = dsc_ll_create(sizeof(char*), (void*)list[0]);
    dsc_ll_add_node(head, (void*)list[1]);
    dsc_ll_add_node(head, (void*)list[2]);

    LLNode_t *tmp = head;
    size_t list_size = sizeof(list) / sizeof(*list);
    ck_assert_int_eq(dsc_ll_num_nodes(head), list_size);

    dsc_ll_remove_node(head, 1);
    ck_assert_int_eq(dsc_ll_num_nodes(head), list_size - 1);
    ck_assert_str_eq(DSC_BUF_AS_CHAR(head->data), list[0]);
    ck_assert_str_eq(DSC_BUF_AS_CHAR(head->next->data), list[2]);

    dsc_ll_destroy(head);
}
END_TEST

START_TEST(RetrieveNode) {
    int i;
    const char* const list[] = { "Foo", "Bar", "Baz" };

    LLNode_t *head = dsc_ll_create(sizeof(char*), (void*)list[0]);
    dsc_ll_add_node(head, (void*)list[1]);
    dsc_ll_add_node(head, (void*)list[2]);

    LLNode_t *first  = dsc_ll_retrieve_node(head, 0);
    LLNode_t *second = dsc_ll_retrieve_node(head, 1);
    LLNode_t *third  = dsc_ll_retrieve_node(head, 2);
    ck_assert_str_eq(DSC_BUF_AS_CHAR(first->data),  list[0]);
    ck_assert_str_eq(DSC_BUF_AS_CHAR(second->data), list[1]);
    ck_assert_str_eq(DSC_BUF_AS_CHAR(third->data),  list[2]);

    dsc_ll_destroy(head);
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
    tcase_add_test(tc_core, RemoveNode);
    tcase_add_test(tc_core, RetrieveNode);
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
