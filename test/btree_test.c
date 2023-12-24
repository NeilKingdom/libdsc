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

static InsertCriteria add_btree_node_insert_func(const pBTreeNode_t comp_to, const pBTreeNode_t node) {
    if (*DSC_BUF_INT(node->data) < *DSC_BUF_INT(comp_to->data)) {
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
    pBTreeNode_t root = dsc_create_btree(sizeof(int), (void*)&nums[4]); /* Root contains 8 */
    pBTreeNode_t *list = malloc(sizeof(pBTreeNode_t) * nums_size);
    ck_assert(list);

    for (i = 0; i < nums_size; ++i) {
        if (i == 4) continue;
        dsc_add_btree_node(root, (void*)&nums[i], add_btree_node_insert_func);
    }

    dsc_get_btree_node_list(root, list, IN_ORDER);
    for (i = 0; i < nums_size; ++i) {
        ck_assert_int_eq(*DSC_BUF_INT(list[i]->data), nums_in_order[i]);
    }

    dsc_destroy_btree(root);
    free(list);
}
END_TEST

static InsertCriteria get_btree_node_insert_func(const pBTreeNode_t comp_to, const pBTreeNode_t node) {
    if (*DSC_BUF_CHAR(node->data) < *DSC_BUF_CHAR(comp_to->data)) {
        return INSERT_GT;
    } else {
        return INSERT_LT;
    }
}

static SortCriteria get_btree_node_sort_func(const pBTreeNode_t node) {
    char key = 'p';

    if (*DSC_BUF_CHAR(node->data) < key) {
        return SORT_LT;
    } else if (*DSC_BUF_CHAR(node->data) > key) {
        return SORT_GT;
    } else {
        return SORT_EQ;
    }
}

START_TEST(GetBTreeNode) {
    int i; 
    char middle = 'n';
    char haystack[] = { 'z', 'q', 'r', 'a', 's', 'p', 'm', 'i', 'c' };
    int hs_size = sizeof(haystack) / sizeof(*haystack);

    pBTreeNode_t root = dsc_create_btree(sizeof(middle), (void*)&middle);
    for (i = 0; i < hs_size; ++i) {
        dsc_add_btree_node(root, (void*)&haystack[i], get_btree_node_insert_func);
    }

    pBTreeNode_t needle = dsc_get_btree_node(root, get_btree_node_sort_func);
    ck_assert_ptr_nonnull(needle);
    ck_assert_int_eq((int)*DSC_BUF_CHAR(needle->data), (int)'p');

    dsc_destroy_btree(root);
}
END_TEST

START_TEST(RemoveBTreeNode) {

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
    tcase_add_test(tc_core, GetBTreeNode);
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
