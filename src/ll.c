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
 *       Private Functions
 * ===============================
 */

/**
 * @brief Free an allocated pLLNode_t struct.
 * @since 06-22-2023
 * @param [in] node A pointer to the node being removed
 * @returns A DscError_t representing the exit status code
 */
static DscError_t _dsc_ll_remove_node(LLNode_t *node) {
    int status;

    if (!node) {
        DSC_LOG("The node points to an invalid address", DSC_ERROR);
        return DSC_EFAULT;
    } else {
        assert((long)node % sysconf(_SC_PAGE_SIZE) == 0);
        status = munmap(node, sizeof(LLNode_t));
        if (status != 0) {
            DSC_LOG("Failed to unmap pLLNode_t struct", DSC_WARNING);
            return DSC_EFAIL;
        }

        node = NULL;
    }

    return DSC_EOK;
}

/*
 * ===============================
 *       Public Functions
 * ===============================
 */

/**
 * @brief Creates the head node of the singly linked list.
 * @since 06-22-2023
 * @param tsize The size (in bytes) of the data type used by this linked list
 * @param data Optional data to initialize the head node with
 * @returns The head node of the linked list, which is used by the other APIs
 */
DSC_DECL LLNode_t *dsc_ll_create(const size_t tsize, const void* const data) {
    LLNode_t *head = NULL;

    /* Allocate space for the head node */
    head = mmap(NULL, sizeof(LLNode_t), (PROT_READ | PROT_WRITE), (MAP_SHARED | MAP_ANON), -1, 0);
    if (head == MAP_FAILED) {
        DSC_LOG("Failed to allocate memory for linked list node", DSC_ERROR);
        return head;
    }

    head->data = dsc_buf_create(1, tsize);
    if (head->data.addr == NULL) {
        return head;
    }

    /* Copy data passed by user to the memory segment */
    if (data != NULL) {
        memcpy(head->data.addr, data, tsize);
    }

    return head;
}

/**
 * @brief Destroys each node in a linked list starting at head.
 * @since 06-22-2023
 * @param head The node at which the deletion will begin until the end of the list
 * @returns A DscError_t representing the exit status code
 */
DSC_DECL DscError_t dsc_destroy_ll(LLNode_t *head) {
    LLNode_t *tmp = head;
    LLNode_t *prev;

    while (tmp->next) {
        prev = tmp;
        tmp = tmp->next;

        dsc_buf_destroy(&prev->data);
        _dsc_ll_remove_node(prev);
    }

    dsc_buf_destroy(&tmp->data);
    _dsc_ll_remove_node(tmp);

    return DSC_EOK;
}

/**
 * @brief Destroys each node in a linked list starting at head.
 * @since 06-22-2023
 * @param head The node at which the deletion will begin until the end of the list
 * @returns A DscError_t representing the exit status code
 */
DSC_DECL DscError_t dsc_ll_fill(LLNode_t *head, const uint8_t byte) {
    int status;
    LLNode_t *tmp = head;

    while (tmp->next) {
        status = dsc_buf_fill(&tmp->data, byte);
        if (status != DSC_EOK) {
            return status;
        }
        tmp = tmp->next;
    }

    return DSC_EOK;
}

DSC_DECL DscError_t dsc_ll_add_node(LLNode_t *head, const void* const data) {
    LLNode_t *new_node = NULL;
    LLNode_t *tmp = head;

    while (tmp->next) {
        tmp = tmp->next;
    }

    /* Allocate space for the node struct */
    new_node = mmap(NULL, sizeof(LLNode_t), (PROT_READ | PROT_WRITE), (MAP_SHARED | MAP_ANON), -1, 0);
    if (new_node == MAP_FAILED) {
        DSC_LOG("Failed to allocate memory for linked list node", DSC_ERROR);
        return DSC_EFAIL;
    }

    /* Allocate space for the node's data */
    new_node->next = NULL;
    new_node->data = dsc_buf_create(1, head->data.tsize);
    if (new_node->data.addr == NULL) {
        return DSC_EFAIL;
    }

    /* Copy data passed by user to the memory segment */
    memcpy(new_node->data.addr, data, head->data.tsize);
    tmp->next = new_node;

    return DSC_EOK;
}

DSC_DECL DscError_t dsc_ll_remove_node(LLNode_t *head, const unsigned idx) {
    int i = 0;
    LLNode_t *tmp = head;
    LLNode_t *prev = tmp;

    while (tmp->next && i < idx) {
        prev = tmp;
        tmp = tmp->next;
        i++;
    }

    if (i == idx) {
        /* Deleting intermediary node */
        if (tmp->next && tmp->next->next) {
            dsc_buf_destroy(&tmp->next->data);
            _dsc_ll_remove_node(tmp->next);
            tmp->next = tmp->next->next;
        /* Deleting last node */
        } else if (tmp->next) {
            dsc_buf_destroy(&tmp->next->data);
            _dsc_ll_remove_node(tmp->next);
            tmp->next = NULL;
        /* Deleting first node */
        } else if (prev == tmp) {
            dsc_buf_destroy(&tmp->data);
            _dsc_ll_remove_node(tmp);
        /* Special case for deleting second node */
        } else {
            dsc_buf_destroy(&tmp->data);
            _dsc_ll_remove_node(tmp);
            prev->next = NULL;
        }
    } else {
        DSC_LOG("Index provided for node removal was outside the bounds of the linked list", DSC_WARNING);
        return DSC_EINVAL;
    }

    return DSC_EOK;
}

DSC_DECL LLNode_t *dsc_ll_retrieve_node(LLNode_t *head, const unsigned idx) {
    int i = 0;
    LLNode_t *tmp = head;

    while (tmp->next && i < idx) {
        tmp = tmp->next;
        i++;
    }

    if (i == idx) {
        return tmp;
    } else {
        DSC_LOG("Index provided for node removal was outside the bounds of the linked list", DSC_WARNING);
        return NULL;
    }
}

DSC_DECL ssize_t dsc_ll_num_nodes(LLNode_t *head) {
    ssize_t i = 1;
    LLNode_t *tmp = head;

    if (head == NULL) {
        DSC_LOG("Invalid value for head provided", DSC_ERROR);
        return -1;
    }

    while (tmp->next) {
        tmp = tmp->next;
        i++;
    }

    return i;
}
