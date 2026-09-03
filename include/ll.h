#ifndef LL_H
#define LL_H

#include "dsc_common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct LLNode {
    void *data;          // Pointer to the node's data
    struct LLNode *next; // Pointer to next node in the list
} LLNode_t;

/**
 * @brief Creates the head node of the singly linked list.
 * @since 06-22-2023
 * @param[in] data Optional data to initialize the head node with.
 * @returns The head node of the linked list, which is used by the other APIs.
 */
static inline LLNode_t *dsc_ll_create(void *data) {
    LLNode_t *head = malloc(sizeof(LLNode_t));
    if (head == NULL) {
        DSC_LOG("Failed to allocate memory for linked list node", DSC_ERROR);
        return NULL;
    }

    head->data = data;
    head->next = NULL;

    return head;
}

/**
 * @brief Destroys each node in a linked list starting at head.
 * @since 06-22-2023
 * @param[in] head The node at which the deletion will begin until the end of the list.
 * @returns A DscError_t representing the exit status code.
 */
static inline DscError_t dsc_ll_destroy(LLNode_t *head) {
    if (head == NULL) {
        DSC_LOG("The node points to an invalid address", DSC_ERROR);
        return DSC_INVAL;
    }

    while (head) {
        LLNode_t *next = head->next;
        free(head);
        head = next;
    }

    return DSC_SUCCESS;
}

/**
 * @brief Add a new node to the end of the linked list.
 * @since 06-22-2023
 * @param[in] head The head node of the linked list.
 * @param[in] data Optional data that the new node will be initialized with.
 * @returns A DscError_t representing the exit status code.
 */
static inline DscError_t dsc_ll_append(LLNode_t **head, void *data) {
    if (head == NULL) {
        DSC_LOG("The node points to an invalid address", DSC_ERROR);
        return DSC_INVAL;
    }

    LLNode_t *new_node = malloc(sizeof(LLNode_t));
    if (new_node == NULL) {
        DSC_LOG("Failed to allocate memory for linked list node", DSC_ERROR);
        return DSC_FAULT;
    }
    new_node->data = data;
    new_node->next = NULL;

    // List is empty
    if (*head == NULL) {
        *head = new_node;
        return DSC_SUCCESS;
    }

    // Traverse to end
    LLNode_t *iter = *head;
    while (iter->next) {
        iter = iter->next;
    }
    iter->next = new_node;

    return DSC_SUCCESS;
}

/**
 * @brief Insert a new node at some offset __idx__
 * @since 03-09-2026
 * @param[in] head The head node of the linked list.
 * @param[in] data Pointer the data that the new node will contain
 * @param[in] idx The 0-based index at which to insert the node
 * @returns A DscError_t representing the exit status code
 */
static inline DscError_t dsc_ll_insert(LLNode_t **head, void *data, const u32 idx) {
    if (head == NULL) {
        DSC_LOG("Node points to an invalid address", DSC_ERROR);
        return DSC_INVAL;
    }

    LLNode_t *new_node = malloc(sizeof(LLNode_t));
    if (new_node == NULL) {
        DSC_LOG("Failed to allocate memory for linked list node", DSC_ERROR);
        return DSC_FAULT;
    }
    new_node->data = data;
    new_node->next = NULL;

    // Insert node before head
    if (idx == 0) {
        new_node->next = *head;
        *head = new_node;
        return DSC_SUCCESS;
    }

    // Traverse to node before insertion idx
    LLNode_t *iter = *head;
    for (u32 i = 0; iter && i < idx - 1; ++i) {
        iter = iter->next;
    }

    if (iter == NULL) {
        DSC_LOG("Index out of bounds in insert", DSC_WARN);
        return DSC_INVAL;
    }

    new_node->next = iter->next;
    iter->next = new_node;

    return DSC_SUCCESS;
}

/**
 * @brief Removes the node at index __idx__
 * @since 06-22-2023
 * @param[in] head The head node of the linked list
 * @param[in] idx The 0-based index of the node you want to remove
 * @returns A DscError_t representing the exit status code
 */
static inline DscError_t dsc_ll_remove(LLNode_t **head, const u32 idx) {
    if (head == NULL || *head == NULL) {
        DSC_LOG("The node points to an invalid address", DSC_ERROR);
        return DSC_INVAL;
    }

    // Remove head (head becomes head->next)
    if (idx == 0) {
        LLNode_t *tmp = *head;
        *head = tmp->next;
        free(tmp);
        return DSC_SUCCESS;
    }

    LLNode_t *iter = *head;
    for (u32 i = 0; iter->next && i < idx - 1; ++i) {
        iter = iter->next;
    }

    if (iter->next == NULL) {
        DSC_LOG("Index out of bounds in remove", DSC_WARN);
        return DSC_INVAL;
    }

    LLNode_t *to_remove = iter->next;
    iter->next = to_remove->next;
    free(to_remove);

    return DSC_SUCCESS;
}

/**
 * @brief Retrieves the node at the index specified by __idx__
 * @since 06-22-2023
 * @param[in] head The head node of the linked list.
 * @param[in] idx The 0-based index of the node you want to retrieve.
 * @returns Returns a read-only view into the node at index __idx__ or NULL if __idx__ is OOB.
 */
static inline const LLNode_t *dsc_ll_peek(const LLNode_t *head, const u32 idx) {
    if (head == NULL) {
        DSC_LOG("The node points to an invalid address", DSC_ERROR);
        return NULL;
    }

    const LLNode_t *iter = head;
    for (u32 i = 0; iter && i < idx; ++i) {
        iter = iter->next;
    }

    if (iter == NULL) {
        DSC_LOG("Index out of bounds in peek", DSC_WARN);
        return NULL;
    }

    return iter;
}

/**
 * @brief Returns the number of nodes contained in the linked list.
 * @warn If head is NULL, this function returns 0.
 * @since 06-22-2023
 * @param[in] head The head node of the linked list.
 * @returns The number of nodes in the linked list.
 */
static inline usize dsc_ll_nelem(const LLNode_t *head) {
    if (head == NULL) {
        DSC_LOG("The node points to an invalid address", DSC_ERROR);
        return 0;
    }

    usize n = 0;
    const LLNode_t *iter = head;
    while (iter) {
        iter = iter->next;
        n++;
    }

    return n;
}

#ifdef __cplusplus
}
#endif

#endif
