#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

#include "include/buffer.h"
#include "include/btree.h"

#define DSC_MIN(x, y) (((x) < (y)) ? (x) : (y))

static InsertCriteria remove_btree_node_insert_func(const pBTreeNode_t comp_to, const pBTreeNode_t node) {
    int i;
    const char *comp_to_as_str = DSC_BUF_CHAR(comp_to->data);
    const char *node_as_str = DSC_BUF_CHAR(node->data);
    size_t min = DSC_MIN(strlen(comp_to_as_str), strlen(node_as_str));

    /* Sort alphabetically */
    for (i = 0; i < min; ++i) {
        if (tolower((int)node_as_str[i]) > tolower((int)comp_to_as_str[i])) {
            return INSERT_GT;
        } else if (tolower((int)node_as_str[i]) < tolower((int)comp_to_as_str[i])) {
            return INSERT_LT;
        } else {
            continue;
        }
    }

    /* Arbitrary decision to return INSERT_LT in case where letters are same up until min indices */
    return INSERT_LT;
}

static SortCriteria remove_btree_node_sort_func(const pBTreeNode_t node) {
    const char *remove = "may";
    
    if (strcmp(remove, DSC_BUF_CHAR(node->data)) < 0) {
        return SORT_LT;
    } else if (strcmp(remove, DSC_BUF_CHAR(node->data)) > 0) {
        return SORT_GT;
    } else {
        return SORT_EQ;
    }
}

int main(void) {
    int i;
    const char *sentence[] = { "a", "sentence", "may", "contain", "many", "words" };
    const char *expected[] = { "a", "sentence", "words" };
    int ssize = sizeof(sentence) / sizeof(*sentence); 
    int esize = sizeof(expected) / sizeof(*expected); 
    pBTreeNode_t root = dsc_create_btree(strlen(sentence[0]) + 1, sizeof(char), (void*)sentence[0]);
    pBTreeNode_t *list = malloc(sizeof(pBTreeNode_t) * esize);

    for (i = 1; i < ssize; ++i) {
        dsc_add_btree_node(root, (void*)sentence[i], strlen(sentence[i]) + 1, remove_btree_node_insert_func);
    } 

    dsc_remove_btree_node(root, remove_btree_node_sort_func);
    dsc_get_btree_node_list(root, list, esize, IN_ORDER);

    for (i = 0; i < esize; ++i) {
        assert(strcmp(DSC_BUF_CHAR(list[i]->data), expected[i]) == 0);
    }
    
    dsc_destroy_btree(root);
    free(list);

    return 0;
}
