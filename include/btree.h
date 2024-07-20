#ifndef BTREE_H
#define BTREE_H

#include "dsc_common.h"
#include "buffer.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct BTreeNode {
    size_t            id;    /* Optional id if you want to have multiple nodes that contain the same data */
    Buffer_t          data;  /* The node's data */
    struct BTreeNode *left;  /* A pointer to the left child node */
    struct BTreeNode *right; /* A pointer to the right child node */
} BTreeNode_t;

typedef enum {
    SORT_LT = -1,
    SORT_EQ =  0,
    SORT_GT =  1,
} SortCriteria_t;

typedef enum {
    INSERT_LT = -1,
    INSERT_GT =  1,
} InsertCriteria_t;

/* Depth-First Search */
typedef enum {
    IN_ORDER,
    PRE_ORDER,
    POST_ORDER,
} DFSMethod_t;

typedef SortCriteria_t      (* const sort_func)(const BTreeNode_t *node);
typedef InsertCriteria_t    (* const insert_func)(const BTreeNode_t *comp_to, const BTreeNode_t *node);

/* Forward function declarations */

DSC_DECL BTreeNode_t        dsc_btree_create(const size_t nelem, const size_t tsize, const void *data);
DSC_DECL DscError_t         dsc_btree_destroy(BTreeNode_t *root);
DSC_DECL DscError_t         dsc_btree_add_node(const BTreeNode_t *root, const void *data, const size_t len, insert_func inf);
DSC_DECL DscError_t         dsc_btree_remove_node(BTreeNode_t *root, sort_func sf);
/* TODO: Add DFSMethod */
DSC_DECL BTreeNode_t        dsc_btree_retrieve_node(const BTreeNode_t *root, sort_func sf);
DSC_DECL BTreeNode_t        dsc_btree_retrieve_node_parent(const BTreeNode_t *root, sort_func sf);
DSC_DECL void               dsc_btree_flatten(const BTreeNode_t *root, BTreeNode_t *list, const size_t lsize, const DFSMethod_t dfs);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* BTREE_H */
