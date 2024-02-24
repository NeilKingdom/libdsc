/**
 * @file btree.c
 * @since 24-01-2024
 * @author Neil Kingdom
 * @version 1.0
 * @brief Provides functions for creating, deleting, and manipulating binary trees
 */

#include "../include/btree.h"

/*
 * ===============================
 *       Private Functions
 * ===============================
 */

static DSC_Error _dsc_add_btree_node(
    restrict pBTreeNode_t root, 
    const restrict pBTreeNode_t 
    new_node, 
    insert_func inf
) {
    switch (inf(root, new_node)) {
        case INSERT_LT: {
            if (!root->left) {
                root->left = new_node;
                return DSC_EOK;
            } else {
                return _dsc_add_btree_node(root->left, new_node, inf);
            }
        }
        case INSERT_GT: {
            if (!root->right) {
                root->right = new_node;
                return DSC_EOK;
            } else {
                return _dsc_add_btree_node(root->right, new_node, inf);
            }
        }
        /* Should not reach */
        default: {
            return DSC_ERROR;
        }
    }
}

static pBTreeNode_t _dsc_get_btree_node(const restrict pBTreeNode_t root, sort_func sf) {
    switch (sf(root)) {
        case SORT_EQ: {
            return root;
        }
        case SORT_LT: {
            if (root->left) {
                return dsc_get_btree_node(root->left, sf);
            } else {
                return NULL;
            }
        }
        case SORT_GT: {
            if (root->right) {
                return dsc_get_btree_node(root->right, sf);
            } else {
                return NULL;
            }
        }
        /* Should not reach */
        default: {
            return NULL;
        }
    }
}

static pBTreeNode_t _dsc_get_btree_node_parent(
    const restrict pBTreeNode_t root, 
    const restrict pBTreeNode_t prev_node,
    sort_func sf
) {
    switch (sf(root)) {
        case SORT_EQ: {
            if (root == prev_node) {
                return NULL;
            } else {
                return prev_node;
            }
        }
        case SORT_LT: {
            if (root->left) {
                return _dsc_get_btree_node_parent(root->left, root, sf);
            } else {
                return NULL;
            }
        }
        case SORT_GT: {
            if (root->right) {
                return _dsc_get_btree_node_parent(root->right, root, sf);
            } else {
                return NULL;
            }
        }
        /* Should not reach */
        default: {
            return NULL;
        }
    }
}

static unsigned idx = 0;

static void _dsc_get_btree_node_list_in_order(
    restrict pBTreeNode_t root, 
    pBTreeNode_t * restrict list, 
    const size_t list_size
) {
    if (!root || idx == list_size) 
        return;

    if (root->left) {
        _dsc_get_btree_node_list_in_order(root->left, list, list_size);
    }
    list[idx++] = root;
    if (root->right) {
        _dsc_get_btree_node_list_in_order(root->right, list, list_size);
    }
}

static void _dsc_get_btree_node_list_pre_order(
    restrict pBTreeNode_t root, 
    pBTreeNode_t * restrict list, 
    const size_t list_size 
) {
    if (!root || idx == list_size) 
        return;

    list[idx++] = root;
    if (root->left) {
        _dsc_get_btree_node_list_pre_order(root->left, list, list_size);
    } 
    if (root->right) {
        _dsc_get_btree_node_list_pre_order(root->right, list, list_size);
    }
}

static void _dsc_get_btree_node_list_post_order(
    restrict pBTreeNode_t root, 
    pBTreeNode_t * restrict list, 
    const size_t list_size 
) {
    if (!root || idx == list_size) 
        return;

    if (root->left) {
        _dsc_get_btree_node_list_post_order(root->left, list, list_size);
    } 
    if (root->right) {
        _dsc_get_btree_node_list_post_order(root->right, list, list_size);
    }
    list[idx++] = root;
}

/*
 * ===============================
 *       Public Functions
 * ===============================
 */

/**
 * @brief Create the root node for a binary tree
 * @since 24-02-2024 
 * @param[in] len The length of the data (usually 1 unless using a string)
 * @param[in] tsize The size of the data's type in bytes
 * @param[in] data A pointer to the initial data used in the root node
 * @returns The newly allocated root node for the tree
 */
DSC_DECL pBTreeNode_t dsc_create_btree(
    const size_t len, 
    const size_t tsize, 
    const void * restrict data
) {
    pBTreeNode_t root = { 0 }; 

    /* Allocate btree node on heap */
    root = mmap(NULL, sizeof(BTreeNode_t), (PROT_READ | PROT_WRITE), (MAP_SHARED | MAP_ANON), -1, 0);
    if (MAP_FAILED == root) {
        DSC_ERROR("Failed to allocate memory for pBTreeNode_t");
        return NULL;
    }

    /* Allocate buffer space for data */
    root->data = dsc_create_buffer(len, tsize);
    if (!root->data.addr) {
        DSC_ERROR("Failed to allocate memory for the node's data");
        return NULL;
    }

    memcpy(root->data.addr, data, len * tsize);

    return root;
}

/**
 * @brief Add a new node to an existing tree
 * @since 24-02-2024 
 * @param[in] root The root node of the tree
 * @param[in] data A pointer to the initial data used in the new node
 * @param[in] len The length of the data (usually 1 unless using a string)
 * @param[in] inf Pointer to the insert function
 * @returns A DSC_Error type corresponding to the exit status
 */
DSC_DECL DSC_Error dsc_add_btree_node(
    restrict pBTreeNode_t root, 
    const void *data, 
    const size_t len, 
    insert_func inf
) {
    if (!root) {
        DSC_ERROR("The node points to an invalid address");
        return DSC_EFAULT;
    }

    pBTreeNode_t new_node = dsc_create_btree(len, root->data.tsize, data);
    return _dsc_add_btree_node(root, new_node, inf);
}

/**
 * @brief Destroy a binary tree
 * @warning You <em>must</em> provide the actual root node.
 *          To remove a child node use dsc_remove_btree_node.
 * @since 24-02-2024 
 * @param[in] root The root node of the tree
 * @returns A DSC_Error type corresponding to the exit status
 */
DSC_DECL DSC_Error dsc_destroy_btree(restrict pBTreeNode_t root) {
    int status;

    if (!root) {
        DSC_ERROR("The node points to an invalid address");
        return DSC_EFAULT;
    } 

    /* Post-order deletion so that root is deleted last */
    if (root->left) {
        dsc_destroy_btree(root->left);
    } 
    if (root->right) {
        dsc_destroy_btree(root->right);
    } 

    dsc_destroy_buffer(&root->data);
    assert((long)root % sysconf(_SC_PAGE_SIZE) == 0);
    status = munmap(root, sizeof(*root));
    if (status != 0) {
        DSC_ERROR("Failed to unmap pBTreeNode_t struct");
        return DSC_EFREE;
    }

    return DSC_EOK;
}

/**
 * @brief Remove a node from an existing tree, alongside its descendants
 * @since 24-02-2024 
 * @param[in] root The root node of the tree
 * @param[in] sf Pointer to the sort function
 * @returns A DSC_Error type corresponding to the exit status
 */
DSC_DECL DSC_Error dsc_remove_btree_node(restrict pBTreeNode_t root, sort_func sf) {
    pBTreeNode_t node = NULL;
    pBTreeNode_t parent = NULL;

    if (!root) {
        DSC_ERROR("The node points to an invalid address");
        return DSC_ERROR;
    }

    node = dsc_get_btree_node(root, sf);

    if (!node) {
        DSC_ERROR("No nodes matching the sort function were found for removal");
        return DSC_ERROR;
    } else {
        /* Reference to node in parent must be set to NULL */
        parent = dsc_get_btree_node_parent(root, sf);
        if (parent) {
            if (parent->left == node) {
                parent->left = NULL;
            } else {
                parent->right = NULL;
            }
        }

        dsc_destroy_btree(node);
    }

    return DSC_EOK;
}

/**
 * @brief Returns a node from the tree matching the criteria of sf if it exists
 * @since 24-02-2024 
 * @param[in] root The root node of the tree
 * @param[in] sf Pointer to the sort function
 * @returns The node if it was found; otherwise returns NULL
 */
DSC_DECL pBTreeNode_t dsc_get_btree_node(const restrict pBTreeNode_t root, sort_func sf) {
    if (!root) {
        DSC_ERROR("The node points to an invalid address");
        return NULL;
    }

    return _dsc_get_btree_node(root, sf);
}

/**
 * @brief Returns a node's parent from the tree matching the criteria of sf if it exists
 * @since 24-02-2024 
 * @param[in] root The root node of the tree
 * @param[in] sf Pointer to the sort function
 * @returns The node's parent if it was found; otherwise returns NULL
 */
DSC_DECL pBTreeNode_t dsc_get_btree_node_parent(const restrict pBTreeNode_t root, sort_func sf) {
    if (!root) {
        DSC_ERROR("The node points to an invalid address");
        return NULL;
    } else if (!root->left && !root->right) {
        DSC_ERROR("The binary tree provided has no children");
        return NULL;
    }

    return _dsc_get_btree_node_parent(root, NULL, sf);
}

/**
 * @brief Flattens the tree into a list ordered according to method 
 * @since 24-02-2024 
 * @param[in] root The root node of the tree
 * @param[in/out] list A pointer to an allocated memory region of 
 *                sufficient size for each node in the tree
 * @param[in] The size of the list in bytes
 * @param[in] method The depth-first-search method by which the list will be ordered
 */
DSC_DECL void dsc_get_btree_node_list(
    const restrict pBTreeNode_t root, 
    pBTreeNode_t * restrict list, 
    const size_t list_size,
    const DFSMethod method
) {
    if (!root) {
        DSC_ERROR("The node points to an invalid address");
        return;
    } else if (!list) {
        DSC_ERROR("The list points to an invalid address");
        return;
    }

    idx = 0;

    switch (method) {
        case IN_ORDER: {
            _dsc_get_btree_node_list_in_order(root, list, list_size);
            break;
        }
        case PRE_ORDER: {
            _dsc_get_btree_node_list_pre_order(root, list, list_size);
            break;
        }
        case POST_ORDER: {
            _dsc_get_btree_node_list_post_order(root, list, list_size);
            break;
        }
    }
}
