#ifndef BTREE_H
#define BTREE_H

#include "dsc_common.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef enum {
    DFS, // Depth-first-search
    BFS  // Breadth-first-search
} SearchMethod_t;

typedef struct BTreeNode {
    size_t id;               // Optional id if you want to have multiple nodes that contain the same data
    void  *data;             // The node's data
    SearchMethod_t method;   // Method of traversing the tree when doing searches (DFS or BFS)
    struct BTreeNode *left;  // A pointer to the left child node
    struct BTreeNode *right; // A pointer to the right child node
} *BTreeNode_t;

typedef enum {
    SEARCH_LT = -1,
    SEARCH_EQ =  0,
    SEARCH_GT =  1
} SearchCmp_t;
typedef SearchCmp_t (* const search_func)(const BTreeNode_t node);

typedef enum {
    INSERT_LT = -1,
    INSERT_GT =  1
} InsertCmp_t;
typedef InsertCmp_t (* const insert_func)(const BTreeNode_t node, const BTreeNode_t cmp);

// Forward function declarations

BTreeNode_t       dsc_btree_create(void *data, size_t *id, const SearchMethod_t method);
DscError_t        dsc_btree_destroy(BTreeNode_t root);
DscError_t        dsc_btree_add(const BTreeNode_t root, void *data, size_t *id, insert_func func);
DscError_t        dsc_btree_remove(BTreeNode_t root, search_func func);
BTreeNode_t       dsc_btree_peek(const BTreeNode_t root, search_func func);
BTreeNode_t       dsc_btree_peek_parent(const BTreeNode_t root, search_func func);
DscError_t        dsc_btree_flatten(const BTreeNode_t root, const BTreeNode_t *list);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // BTREE_H
