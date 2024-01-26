#ifndef BTREE_H
#define BTREE_H

#include "common.h"
#include "buffer.h"

typedef struct BTreeNode {
    size_t            id;
    buffer_t          data;
    struct BTreeNode *left;
    struct BTreeNode *right;
} *pBTreeNode_t, BTreeNode_t;

typedef enum {
    SORT_LT = -1,
    SORT_EQ,
    SORT_GT,
} SortCriteria;

typedef enum {
    INSERT_LT = -1,
    INSERT_GT =  1,
} InsertCriteria;

typedef enum {
    IN_ORDER = 0, 
    PRE_ORDER,
    POST_ORDER,
} DFSMethod; /* Depth-First Search */

typedef SortCriteria (* const sort_func)(const pBTreeNode_t node);
typedef InsertCriteria (* const insert_func)(const pBTreeNode_t comp_to, const pBTreeNode_t node);

static size_t next_id;

DSC_DECL pBTreeNode_t   dsc_create_btree(const size_t len, const size_t tsize, void *data);
DSC_DECL DSC_Error      dsc_add_btree_node(const pBTreeNode_t root, void *data, const size_t len, insert_func inf);
/* TODO: Add DFSMethod */
DSC_DECL pBTreeNode_t   dsc_get_btree_node(const pBTreeNode_t root, sort_func sf);
DSC_DECL void           dsc_get_btree_node_list(const pBTreeNode_t root, pBTreeNode_t *list, const size_t list_size, const DFSMethod method);
DSC_DECL pBTreeNode_t   dsc_get_btree_node_parent(const pBTreeNode_t root, sort_func sf);
DSC_DECL DSC_Error      dsc_remove_btree_node(pBTreeNode_t root, sort_func sf);
DSC_DECL DSC_Error      dsc_destroy_btree(pBTreeNode_t root);

#endif /* BTREE_H */
