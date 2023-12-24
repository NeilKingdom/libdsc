#define _GNU_SOURCE
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>

#include "../include/btree.h"

DSC_DECL pBTreeNode_t dsc_create_btree(const size_t tsize, void *data) {
    pBTreeNode_t root = { 0 }; 

    /* Allocate btree node on heap */
    root = mmap(NULL, sizeof(BTreeNode_t), (PROT_READ | PROT_WRITE), (MAP_SHARED | MAP_ANON), -1, 0);
    if (MAP_FAILED == root) {
        DSC_ERROR("Failed to allocate memory for pBTreeNode_t");
        return NULL;
    }

    /* Allocate buffer space for data */
    root->data = dsc_create_buffer(1, tsize);
    if (root->data.addr == NULL) {
        DSC_ERROR("Failed to allocate memory for the node's data");
        return NULL;
    }

    root->id = next_id++;
    memcpy(root->data.addr, data, tsize);

    return root;
}

DSC_DECL DSC_Error dsc_destroy_btree(pBTreeNode_t root) {
    int status;

    if (!root) {
        DSC_ERROR("The node points to an invalid address");
        return DSC_EFAULT;
    } 

    /* Post-order deletion so that root is deleted last */
    if (root->left != NULL) {
        dsc_destroy_btree(root->left);
    } else if (root->right != NULL) {
        dsc_destroy_btree(root->right);
    } else {
        dsc_destroy_buffer(&root->data);
        assert((long)root % sysconf(_SC_PAGE_SIZE) == 0);
        status = munmap(root, sizeof(*root));
        if (status != 0) {
            DSC_ERROR("Failed to unmap pBTreeNode_t struct");
            return DSC_EFREE;
        }
    }

    return DSC_EOK;
}

static DSC_Error _dsc_add_btree_node(const pBTreeNode_t root, const pBTreeNode_t new_node, insert_func inf) {
    InsertCriteria sc = inf(root, new_node); 

    switch (sc) {
        case INSERT_LT: {
            if (root->left == NULL) {
                root->left = new_node;
                return DSC_EOK;
            } else {
                _dsc_add_btree_node(root->left, new_node, inf);
                break;
            }
        }
        case INSERT_GT: {
            if (root->right == NULL) {
                root->right = new_node;
                return DSC_EOK;
            } else {
                _dsc_add_btree_node(root->right, new_node, inf);
                break;
            }
        }
    }

    return DSC_ERROR;
}

DSC_DECL DSC_Error dsc_add_btree_node(const pBTreeNode_t root, void *data, insert_func inf) {
    if (!root) {
        DSC_ERROR("The node points to an invalid address");
        return DSC_EFAULT;
    }

    pBTreeNode_t new_node = dsc_create_btree(root->data.tsize, data);
    return _dsc_add_btree_node(root, new_node, inf);
}

DSC_DECL DSC_Error dsc_remove_btree_node(restrict pBTreeNode_t root, sort_func sf) {
    if (!root) {
        DSC_ERROR("The node points to an invalid address");
        return DSC_ERROR;
    }

    pBTreeNode_t node = dsc_get_btree_node(root, sf);
    if (node == NULL) {
        DSC_ERROR("No nodes matching the sort function were found for removal");
        return DSC_ERROR;
    } else {
        dsc_destroy_btree(node);
    }

    return DSC_EOK;
}

DSC_DECL pBTreeNode_t dsc_get_btree_node(const restrict pBTreeNode_t root, sort_func sf) {
    SortCriteria sc = sf(root);

    /* Could not find node specified */
    if (!root ||
        (sc == SORT_LT && root->left == NULL) ||
        (sc == SORT_GT && root->right == NULL)) {
        return NULL;
    }

    switch (sc) {
        case SORT_EQ: {
            return root;
        }
        case SORT_LT: {
            dsc_get_btree_node(root->left, sf);
            break;
        }
        case SORT_GT: {
            dsc_get_btree_node(root->right, sf);
            break;
        }
    }

    return NULL;
}

/* TODO: Implement */
static pBTreeNode_t _dsc_get_parent_btree_node(
    const restrict pBTreeNode_t root, 
    const restrict pBTreeNode_t child
) {
    return NULL;    
}

DSC_DECL pBTreeNode_t dsc_get_btree_node_parent(const restrict pBTreeNode_t root, sort_func sf) {
    if (!root) {
        DSC_ERROR("The node points to an invalid address");
        return NULL;
    }

    pBTreeNode_t child = dsc_get_btree_node(root, sf);
    if (!child) {
        return NULL;
    }
    
    return _dsc_get_parent_btree_node(root, child);
}

/* TODO: This is just a temporary solution */
static unsigned idx;

static void _dsc_get_btree_node_list_in_order(restrict pBTreeNode_t root, pBTreeNode_t * restrict list) {
    if (!root) return;

    _dsc_get_btree_node_list_in_order(root->left, list);
    list[idx++] = root;
    _dsc_get_btree_node_list_in_order(root->right, list);
}

static void _dsc_get_btree_node_list_pre_order(restrict pBTreeNode_t root, pBTreeNode_t * restrict list) {
    if (!root) return;

    list[idx++] = root;
    _dsc_get_btree_node_list_pre_order(root->left, list);
    _dsc_get_btree_node_list_pre_order(root->right, list);
}

static void _dsc_get_btree_node_list_post_order(restrict pBTreeNode_t root, pBTreeNode_t * restrict list) {
    if (!root) return;

    _dsc_get_btree_node_list_post_order(root->left, list);
    _dsc_get_btree_node_list_post_order(root->right, list);
    list[idx++] = root;
}

DSC_DECL void dsc_get_btree_node_list(
    const restrict pBTreeNode_t root, 
    pBTreeNode_t * restrict list, 
    const DFSMethod method
) {
    if (!root) {
        DSC_ERROR("The node points to an invalid address");
        return;
    }

    idx = 0;

    switch (method) {
        case IN_ORDER: {
            _dsc_get_btree_node_list_in_order(root, list);
            break;
        }
        case PRE_ORDER: {
            _dsc_get_btree_node_list_pre_order(root, list);
            break;
        }
        case POST_ORDER: {
            _dsc_get_btree_node_list_post_order(root, list);
            break;
        }
    }
}

DSC_DECL size_t dsc_get_next_btree_id() {
    return next_id;
}
