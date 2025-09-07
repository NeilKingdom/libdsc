/**
 * @file ll.c
 * @author Neil Kingdom
 * @version 1.0
 * @since 06-22-2023
 * @brief Provides APIs for managing a singly linked list.
*/

#include "ll.h"

/*
 * ===============================
 *       Public Functions
 * ===============================
 */

/**
 * @brief Creates the head node of the singly linked list.
 * @since 06-22-2023
 * @param[in] tsize The size (in bytes) of the data type used by this linked list
 * @param[in] data Optional data to initialize the head node with
 * @returns The head node of the linked list, which is used by the other APIs
 */
LLNode_t dsc_ll_create(void* data) {
    LLNode_t head = NULL;

    head = malloc(sizeof(struct LLNode));
    if (head == NULL) {
        DSC_LOG("Failed to allocate memory for dsc linked list node", DSC_ERROR);
        return NULL;
    }
    head->data = data;
    head->next = NULL;

    return head;
}

/**
 * @brief Destroys each node in a linked list starting at head.
 * @since 06-22-2023
 * @param[in] head The node at which the deletion will begin until the end of the list
 * @returns A DscError_t representing the exit status code
 */
DscError_t dsc_ll_destroy(LLNode_t head) {
    LLNode_t iter = head;
    LLNode_t prev;

    if (head == NULL) {
        DSC_LOG("The node points to an invalid address", DSC_ERROR);
        return DSC_EINVAL;
    }

    while (iter->next) {
        prev = iter;
        iter = iter->next;
        free(prev);
    }
    free(iter);

    return DSC_EOK;
}

/**
 * @brief Add a new node to the end of the linked list.
 * @since 06-22-2023
 * @param[in] head The head node of the linked list
 * @param[in] data Optional data that the new node will be initialized with
 * @returns A DscError_t representing the exit status code
 */
DscError_t dsc_ll_append(LLNode_t head, void* data) {
    LLNode_t new_node = NULL;
    LLNode_t iter = head;

    if (head == NULL) {
        DSC_LOG("The node points to an invalid address", DSC_ERROR);
        return DSC_EINVAL;
    }

    while (iter->next) {
        iter = iter->next;
    }

    new_node = malloc(sizeof(struct LLNode));
    if (new_node == NULL) {
        DSC_LOG("Failed to allocate memory for dsc linked list node", DSC_ERROR);
        return DSC_EFAULT;
    }
    new_node->data = data;
    new_node->next = NULL;

    iter->next = new_node;

    return DSC_EOK;
}

DscError_t dsc_ll_insert(LLNode_t restrict head, const LLNode_t restrict node, const unsigned idx) {
    unsigned i = 0;
    LLNode_t iter = head;
    LLNode_t prev = iter;

    if (head == NULL) {
        DSC_LOG("The node points to an invalid address", DSC_ERROR);
        return DSC_EINVAL;
    }

    if (dsc_ll_nelem(head) < 2) {
        DSC_LOG("Attempted insertion on linked list smaller than 2 elements. Did you mean to append?", DSC_WARNING);
        return DSC_EINVAL;
    }

    while (iter->next && i++ < idx) {
        prev = iter;
        iter = iter->next;
    }

    if (i == idx) {
        prev->next = node;
        node->next = iter;
    } else {
        DSC_LOG("Index provided for node removal was outside the bounds of the linked list", DSC_WARNING);
        return DSC_EINVAL;
    }

    return DSC_EOK;
}

/**
 * @brief Removes the node at index "idx".
 * @since 06-22-2023
 * @param[in] head The head node of the linked list
 * @param[in] idx The 0-based index of the node you want to remove
 * @returns A DscError_t representing the exit status code
 */
DscError_t dsc_ll_remove(LLNode_t head, const unsigned idx) {
    unsigned i = 0;
    LLNode_t iter = head;
    LLNode_t prev = iter;

    if (head == NULL) {
        DSC_LOG("The node points to an invalid address", DSC_ERROR);
        return DSC_EINVAL;
    }

    while (iter->next && i++ < idx) {
        prev = iter;
        iter = iter->next;
    }

    if (i == idx) {
        prev->next = iter->next;
        dsc_ll_destroy(iter);
    } else {
        DSC_LOG("Index provided for node removal was outside the bounds of the linked list", DSC_WARNING);
        return DSC_EINVAL;
    }

    return DSC_EOK;
}

/**
 * @brief Retrieves the node at index "idx".
 * @since 06-22-2023
 * @param[in] head The head node of the linked list
 * @param[in] idx The 0-based index of the node you want to retrieve
 * @returns NULL upon failure, otherwise returns the node located at index "idx"
 */
LLNode_t dsc_ll_peek(const LLNode_t head, const unsigned idx) {
    unsigned i = 0;
    LLNode_t iter = head;

    if (head == NULL) {
        DSC_LOG("The node points to an invalid address", DSC_ERROR);
        return NULL;
    }

    while (iter->next && i++ < idx) {
        iter = iter->next;
    }

    if (i == idx) {
        return iter;
    } else {
        DSC_LOG("Index provided for node removal was outside the bounds of the linked list", DSC_WARNING);
        return NULL;
    }
}

/**
 * @brief Returns the number of nodes contained in the linked list.
 * @since 06-22-2023
 * @param[in] head The head node of the linked list
 * @returns -1 upon failure, otherwise the number of nodes in the list
 */
size_t dsc_ll_nelem(const LLNode_t head) {
    size_t n;
    LLNode_t iter = head;

    if (head == NULL) {
        DSC_LOG("The node points to an invalid address", DSC_ERROR);
        return -1;
    }

    for (n = 1; iter->next; ++n) {
        iter = iter->next;
    }

    return n;
}
