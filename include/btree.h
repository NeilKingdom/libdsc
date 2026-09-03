#ifndef BTREE_H
#define BTREE_H

#include "dsc_common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct BTreeNode {
    void  *data;             // The node's data
    struct BTreeNode *left;  // A pointer to the left child node
    struct BTreeNode *right; // A pointer to the right child node
} BTreeNode_t;

// Allows user to specify how data is compared for tree traversal
// NOTE: In the case of insert_fn, CMP_EQ is interpreted as 'replace the node'
typedef enum {
    CMP_LT = -1,
    CMP_EQ =  0,
    CMP_GT =  1
} CmpResult_t;
typedef CmpResult_t (*search_fn)(const BTreeNode_t *root);
typedef CmpResult_t (*insert_fn)(BTreeNode_t *root, BTreeNode_t *new);

/*
 * ===============================
 *       Private Functions
 * ===============================
 */

static DscError_t _dsc_btree_add(
    BTreeNode_t **root,
    BTreeNode_t *new,
    insert_fn fn
) {
    switch (fn(*root, new)) {
        case CMP_EQ:
            new->left  = (*root)->left;
            new->right = (*root)->right;

            free(*root);
            *root = new;

            return DSC_SUCCESS;
        case CMP_LT:
            if ((*root)->left == NULL) {
                (*root)->left = new;
                return DSC_SUCCESS;
            }
            return _dsc_btree_add(&(*root)->left, new, fn);
        case CMP_GT:
            if ((*root)->right == NULL) {
                (*root)->right = new;
                return DSC_SUCCESS;
            }
            return _dsc_btree_add(&(*root)->right, new, fn);
    }

    return DSC_FAILURE;
}

static BTreeNode_t **_dsc_btree_find_node(BTreeNode_t **root, search_fn fn) {
    if (root == NULL || *root == NULL) {
        return NULL;
    }

    switch (fn(*root)) {
        case CMP_EQ:
            return root;
        case CMP_LT:
            return _dsc_btree_find_node(&(*root)->left, fn);
        case CMP_GT:
            return _dsc_btree_find_node(&(*root)->right, fn);
    }

    return NULL;
}

/*
 * ===============================
 *       Public Functions
 * ===============================
 */

/**
 * @brief Create the root node for a binary tree.
 * @since 24-02-2024
 * @param[in] data A pointer to the initial data used in the root node
 * @returns The newly allocated root node for the tree
 */
static inline BTreeNode_t *dsc_btree_create(void *data) {
    BTreeNode_t *root = malloc(sizeof(BTreeNode_t));
    if (root == NULL) {
        DSC_LOG("Failed to allocate memory for dsc btree node", DSC_ERROR);
        return NULL;
    }

    root->data = data;
    root->left = NULL;
    root->right = NULL;

    return root;
}

/**
 * @brief Destroy a binary tree.
 * @warning You <em>must</em> provide the actual root node.
 * To remove a child node use dsc_btree_remove_subtree().
 * @since 24-02-2024
 * @param[in] root The root node of the tree
 * @returns A DscError_t type corresponding to the exit status
 */
static inline DscError_t dsc_btree_destroy(BTreeNode_t **root) {
    if (root == NULL || *root == NULL) {
        DSC_LOG("The node points to an invalid address", DSC_ERROR);
        return DSC_INVAL;
    }

    // Post-order deletion so that root is deleted last
    if ((*root)->left != NULL) {
        dsc_btree_destroy(&(*root)->left);
    }
    if ((*root)->right != NULL) {
        dsc_btree_destroy(&(*root)->right);
    }

    free(*root);
    *root = NULL;

    return DSC_SUCCESS;
}

/**
 * @brief Add a new node to the binary tree.
 * @since 24-02-2024
 * @param[in] root The root node of the binary tree
 * @param[in] data A pointer to the initial data used in the new node
 * @param[in] fn Insert comparison function/hook
 * @returns A DscError_t type corresponding to the exit status
 */
static inline DscError_t dsc_btree_add(BTreeNode_t **root, void *data, insert_fn fn) {
    if (root == NULL || *root == NULL) {
        DSC_LOG("The node points to an invalid address", DSC_ERROR);
        return DSC_INVAL;
    }

    BTreeNode_t *new = dsc_btree_create(data);
    if (new == NULL) {
        return DSC_FAILURE;
    }

    return _dsc_btree_add(root, new, fn);
}

/**
 * @brief Remove a node and all of its descendants from an existing tree.
 * @since 24-02-2024
 * @param[in] root The root node of the tree
 * @param[in] fn Search comparison function/hook
 * @returns A DscError_t type corresponding to the exit status
 */
static inline DscError_t dsc_btree_remove_subtree(BTreeNode_t **root, search_fn fn) {
    if (root == NULL || *root == NULL) {
        return DSC_INVAL;
    }

    BTreeNode_t **needle = _dsc_btree_find_node(root, fn);
    if (needle == NULL) {
        return DSC_FAILURE;
    }

    return dsc_btree_destroy(needle);
}

/**
 * @brief Returns a node from the tree matching the criteria of fn if it exists.
 * @since 24-02-2024
 * @param[in] root The root node of the tree
 * @param[in] fn Search comparison function/hook
 * @returns The node if it was found; otherwise returns NULL
 */
static inline BTreeNode_t *dsc_btree_peek(BTreeNode_t *root, search_fn fn) {
    if (root == NULL) {
        return NULL;
    }

    BTreeNode_t **needle = _dsc_btree_find_node(&root, fn);
    return needle ? *needle : NULL;
}

#ifdef __cplusplus
}
#endif

#endif
