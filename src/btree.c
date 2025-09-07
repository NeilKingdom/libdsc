/**
 * @file btree.c
 * @since 24-01-2024
 * @author Neil Kingdom
 * @version 1.0
 * @brief Provides functions for creating, deleting, and manipulating binary trees.
 */

#include "btree.h"

/*
 * ===============================
 *       Private Functions
 * ===============================
 */

static DscError_t _dsc_btree_add(
    BTreeNode_t root,
    const BTreeNode_t new_node,
    insert_func func
) {
    switch (func(root, new_node)) {
        case INSERT_LT: {
            if (root->left == NULL) {
                root->left = new_node;
                return DSC_EOK;
            } else {
                return _dsc_btree_add(root->left, new_node, func);
            }
        }
        case INSERT_GT: {
            if (root->right == NULL) {
                root->right = new_node;
                return DSC_EOK;
            } else {
                return _dsc_btree_add(root->right, new_node, func);
            }
        }
        default: {
            DSC_LOG("Invalid branch arm", DSC_ERROR);
            return DSC_EFAIL;
        }
    }
}

static BTreeNode_t _dsc_btree_peek_dfs(const BTreeNode_t root, search_func func) {
    switch (func(root)) {
        case SEARCH_EQ: {
            return root;
        }
        case SEARCH_LT: {
            if (root->left != NULL) {
                return _dsc_btree_peek_dfs(root->left, func);
            } else {
                return NULL;
            }
        }
        case SEARCH_GT: {
            if (root->right != NULL) {
                return _dsc_btree_peek_dfs(root->right, func);
            } else {
                return NULL;
            }
        }
        default: {
            DSC_LOG("Invalid branch arm", DSC_ERROR);
            return NULL;
        }
    }
}

static BTreeNode_t _dsc_btree_peek_parent_dfs(
    const BTreeNode_t root,
    const BTreeNode_t prev_node,
    search_func func
) {
    switch (func(root)) {
        case SEARCH_EQ: {
            if (root == prev_node) {
                DSC_LOG("Tried finding parent for the root node", DSC_WARNING);
                return NULL;
            } else {
                return prev_node;
            }
        }
        case SEARCH_LT: {
            if (root->left != NULL) {
                return _dsc_btree_peek_parent_dfs(root->left, root, func);
            } else {
                return NULL;
            }
        }
        case SEARCH_GT: {
            if (root->right != NULL) {
                return _dsc_btree_peek_parent_dfs(root->right, root, func);
            } else {
                return NULL;
            }
        }
        default: {
            DSC_LOG("Invalid branch arm", DSC_ERROR);
            return NULL;
        }
    }
}

/*
 * ===============================
 *       Public Functions
 * ===============================
 */

/**
 * @brief Create the root node for a binary tree.
 * @since 24-02-2024
 * @param[in] nelem The number of elements for the buffer (usually 1 unless using a string)
 * @param[in] tsize The size of the data's type in bytes
 * @param[in] data A pointer to the initial data used in the root node
 * @returns The newly allocated root node for the tree
 */
BTreeNode_t dsc_btree_create(
    void *data,
    size_t *id,
    const SearchMethod_t method
) {
    BTreeNode_t root = NULL;

    root = malloc(sizeof(struct BTreeNode));
    if (root == NULL) {
        DSC_LOG("Failed to allocate memory for dsc btree node", DSC_ERROR);
        return NULL;
    }

    if (id != NULL) {
        root->id = *id;
    } else {
        root->id = 0;
    }
    root->data = data;
    root->method = method;
    root->left = NULL;
    root->right = NULL;

    return root;
}

/**
 * @brief Destroy a binary tree.
 * @warning You <em>must</em> provide the actual root node.
 * To remove a child node use dsc_remove_btree_node().
 * @since 24-02-2024
 * @param[in] root The root node of the tree
 * @returns A DscError_t type corresponding to the exit status
 */
DscError_t dsc_btree_destroy(BTreeNode_t root) {
    if (root == NULL) {
        DSC_LOG("The node points to an invalid address", DSC_ERROR);
        return DSC_EINVAL;
    }

    // Post-order deletion so that root is deleted last
    if (root->left != NULL) {
        dsc_btree_destroy(root->left);
    }
    if (root->right != NULL) {
        dsc_btree_destroy(root->right);
    }
    free(root);

    return DSC_EOK;
}

/**
 * @brief Add a new node to the binary tree.
 * @since 24-02-2024
 * @param[in] root The root node of the binary tree
 * @param[in] data A pointer to the initial data used in the new node
 * @param[in] nelem The number of elements for the buffer (usually 1 unless using a string)
 * @param[in] inf Pointer to the insert function
 * @returns A DSC_Error type corresponding to the exit status
 */
DscError_t dsc_btree_add(
    BTreeNode_t root,
    void *data,
    size_t *id,
    insert_func func
) {
    if (root == NULL) {
        DSC_LOG("The node points to an invalid address", DSC_ERROR);
        return DSC_EINVAL;
    }

    BTreeNode_t new_node = dsc_btree_create(data, id, root->method);
    return _dsc_btree_add(root, new_node, func);
}

/**
 * @brief Remove a node from an existing tree, alongside its descendants.
 * @since 24-02-2024
 * @param[in] root The root node of the tree
 * @param[in] sf Pointer to the sort function
 * @returns A DscError_t type corresponding to the exit status
 */
DscError_t dsc_btree_remove(BTreeNode_t root, search_func func) {
    BTreeNode_t node = NULL;
    BTreeNode_t parent = NULL;

    if (root == NULL) {
        DSC_LOG("The node points to an invalid address", DSC_ERROR);
        return DSC_EINVAL;
    }

    node = dsc_btree_peek(root, func);

    if (node == NULL) {
        DSC_LOG("No matching nodes were found for removal", DSC_WARNING);
        return DSC_EFAIL;
    } else {
        // We should inform the parent that its child has been deleted
        parent = dsc_btree_peek_parent(root, func);
        if (parent != NULL) {
            if (parent->left == node) {
                parent->left = NULL;
            } else {
                parent->right = NULL;
            }
        }

        dsc_btree_destroy(node);
    }

    return DSC_EOK;
}

/**
 * @brief Returns a node from the tree matching the criteria of sf if it exists.
 * @since 24-02-2024
 * @param[in] root The root node of the tree
 * @param[in] sf Pointer to the sort function
 * @returns The node if it was found; otherwise returns NULL
 */
BTreeNode_t dsc_btree_peek(BTreeNode_t root, search_func func) {
    if (root == NULL) {
        DSC_LOG("The node points to an invalid address", DSC_ERROR);
        return NULL;
    }

    if (root->method == DFS) {
        return _dsc_btree_peek_dfs(root, func);
    } else {
        DSC_LOG("Implement me", DSC_ERROR);
        assert(false);
    }
}

/**
 * @brief Returns a node's parent from the tree matching the criteria of sf if it exists.
 * @since 24-02-2024
 * @param[in] root The root node of the tree
 * @param[in] sf Pointer to the sort function
 * @returns The node's parent if it was found; otherwise returns NULL
 */
BTreeNode_t dsc_btree_peek_parent(BTreeNode_t root, search_func func) {
    if (root == NULL) {
        DSC_LOG("The node points to an invalid address", DSC_ERROR);
        return NULL;
    } else if (root->left == NULL && root->right == NULL) {
        DSC_LOG("The binary tree provided has no children", DSC_WARNING);
        return NULL;
    }

    if (root->method == DFS) {
        return _dsc_btree_peek_parent_dfs(root, NULL, func);
    } else {
        DSC_LOG("Implement me", DSC_ERROR);
        assert(false);
    }
}

/**
 * @brief Flattens the tree into a list ordered according to method.
 * @since 24-02-2024
 * @param[in] root The root node of the tree
 * @param[in/out] list A pointer to an allocated memory region of
 *                sufficient size for each node in the tree
 * @param[in] The size of the list in bytes
 * @param[in] method The depth-first-search method by which the list will be ordered
 */
DscError_t dsc_btree_flatten(const BTreeNode_t root, const BTreeNode_t *list) {
    if (root == NULL) {
        DSC_LOG("The node points to an invalid address", DSC_ERROR);
        return DSC_EINVAL;
    }

    if (list == NULL) {
        DSC_LOG("The btree list points to an invalid address", DSC_ERROR);
        return DSC_EINVAL;
    }

    DSC_LOG("Implement me", DSC_ERROR);
    assert(false);
}
