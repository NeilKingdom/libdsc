#include <stdio.h>
#include <stdlib.h>
#include <check.h>

#include "../include/btree.h"
#include "../include/buffer.h"

START_TEST(CreateBTree) {
    char *greeting = "Hello, World";
    pBTreeNode_t root = dsc_create_btree(strlen(greeting) + 1, greeting);
    ck_assert_str_eq(DSC_BUF_CHAR(root->data), greeting);
    ck_assert_int_eq((int)root->id, 0);
    ck_assert_int_eq((int)dsc_get_next_btree_id(), 1);
    dsc_destroy_btree(root);
}
END_TEST

static InsertCriteria add_btree_node_sort_func(const pBTreeNode_t comp_to, const pBTreeNode_t node) {
    if (DSC_BUF_INT(node->data) < DSC_BUF_INT(comp_to->data)) {
        return INSERT_LT;
    } else {
        return INSERT_GT;
    }
}

START_TEST(AddBTreeNode) {
    int i;
    int nums[] = { 5, 2, 10, 7, 8, 21, 3 };
    int nums_size = sizeof(nums) / sizeof(*nums);
    int nums_in_order[] = { 2, 3, 5, 7, 8, 10, 21 };
    pBTreeNode_t root = dsc_create_btree(sizeof(int), &nums[4]); /* Root contains 8 */
    pBTreeNode_t *list = malloc(sizeof(pBTreeNode_t) * nums_size);
    ck_assert(list);

    for (i = 0; i < nums_size - 1; ++i) {
        if (i == 4) continue;
        dsc_add_btree_node(root, &nums[i], add_btree_node_sort_func);
    }

    dsc_get_btree_node_list(root, list, IN_ORDER);
    for (i = 0; i < nums_size; ++i) {
        ck_assert_int_eq(*DSC_BUF_INT(list[i]->data), nums_in_order[i]);
    }

    dsc_destroy_btree(root);
}
END_TEST

START_TEST(GetNode) {
}
END_TEST

Suite *buffer_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("BTree");

    /* Core test cases */
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, CreateBTree);
    tcase_add_test(tc_core, AddBTreeNode);
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
