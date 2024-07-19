#include "tree.h"

DSC_DECL DscError_t dsc_add_tree_node(pTreeNode_t parent, pTreeNode_t child) {
    return DSC_EFAIL;
}

DSC_DECL pTreeNode_t dsc_get_prev_tree_node(void) {
    return prev_node;
}

DSC_DECL pTreeNode_t dsc_get_parent_tree_node(pTreeNode_t restrict child) {
    return NULL;
}

DSC_DECL pTreeNode_t *dsc_get_tree_node_siblings(pTreeNode_t restrict node) {
    return NULL;
}
