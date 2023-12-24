#ifndef TREE_H
#define TREE_H

#include "common.h"
#include "buffer.h"

typedef struct TreeNode {
    buffer_t          data; 
    struct TreeNode **children;
} *pTreeNode_t, TreeNode_t;

typedef enum {
    SEARCH_LEFT  = -1,
    SEARCH_FOUND =  0,
    SEARCH_RIGHT =  1,
} SearchCriteria;

typedef SearchCriteria (*search_func)(pTreeNode_t node);

static pTreeNode_t prev_node;

DSC_DECL DSC_Error      dsc_add_tree_node(pTreeNode_t parent, pTreeNode_t child);
DSC_DECL pTreeNode_t    dsc_get_tree_node(pTreeNode_t node, search_func sf);
DSC_DECL pTreeNode_t    dsc_get_prev_tree_node(void);
DSC_DECL pTreeNode_t    dsc_get_parent_tree_node(pTreeNode_t child);
DSC_DECL pTreeNode_t   *dsc_get_tree_node_siblings(pTreeNode_t node);

#endif /* TREE_H */
