/**
 * This file provides functionality for building a
 * standard linked list. Definitions for doubly
 * linked lists are provided in dll.c.
 *
 * @file
 * @author Neil Kingdom
 * @version 1.0
 * @since 06/22/2023
*/
#define _GNU_SOURCE
#include <assert.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/mman.h>

#include "../include/buffer.h"
#include "../include/ll.h"

/*
 * ===============================
 *       Private Functions
 * ===============================
 */

/**
 * Free an allocated pLLNode_t struct.
 *
 * @since 06/22/2023
 * @param [in] np A pointer to a LLNode_t
 * @returns A status code representing the state of operations upon completion
 */
static int _dsc_del_ll_node(pLLNode_t np) {
   int status;

   if (!np) {
      DSC_ERROR("The node points to an invalid address");
      return DSC_EFAULT;
   } else {
      assert((long)np % sysconf(_SC_PAGE_SIZE) == 0);
      status = munmap(np, sizeof(LLNode_t));
      if (status != 0) {
         DSC_ERROR("Failed to unmap pLLNode_t struct");
         return DSC_EFREE;
      }

      np = NULL;
   }

   return DSC_EOK;
}

/*
 * ===============================
 *       Public Functions
 * ===============================
 */

DSC_DECL pLLNode_t dsc_create_ll(const size_t tsize, void *data) {
   pLLNode_t head = NULL;

   /* Allocate space for the node struct */
   head = mmap(NULL, sizeof(LLNode_t), (PROT_READ | PROT_WRITE), (MAP_SHARED | MAP_ANON), -1, 0);
   if (MAP_FAILED == head) {
      DSC_ERROR("Failed to allocate memory for pLLNode_t");
      return NULL;
   }

   head->next = NULL;
   head->data = dsc_create_buffer(1, tsize);
   if (head->data.addr == NULL) {
      DSC_ERROR("Failed to allocate memory for the node's data");
      return NULL;
   }

   /* Copy data passed by user to the memory segment */
   memcpy(head->data.addr, data, tsize);

   return head;
}

DSC_DECL DSC_Error dsc_add_ll_node(pLLNode_t head, void *data) {
   pLLNode_t new_node = NULL;
   pLLNode_t tmp = head;

   while (tmp->next) {
      tmp = tmp->next;
   }

   /* Allocate space for the node struct */
   new_node = mmap(NULL, sizeof(LLNode_t), (PROT_READ | PROT_WRITE), (MAP_SHARED | MAP_ANON), -1, 0);
   if (MAP_FAILED == new_node) {
      DSC_ERROR("Failed to allocate memory for pLLNode_t");
      return DSC_ERROR;
   }

   /* Allocate space for the node's data */
   new_node->next = NULL;
   new_node->data = dsc_create_buffer(1, head->data.tsize);
   if (new_node->data.addr == NULL) {
      DSC_ERROR("Failed to allocate memory for the node's data");
      return DSC_ERROR;
   }

   /* Copy data passed by user to the memory segment */
   memcpy(new_node->data.addr, data, head->data.tsize);

   tmp->next = new_node;

   return DSC_EOK;
}

/* TODO: Doesn't account for deleting head */
DSC_DECL DSC_Error dsc_del_ll_node(pLLNode_t head, const unsigned index) {
   int i = 0;
   pLLNode_t tmp = head;
   pLLNode_t prev = tmp; /* Keep track of previous node in case we need to delete last node */

   while (tmp->next && i < index) {
      prev = tmp;
      tmp = tmp->next;
      i++;
   }

   if (i == index) {
      if (tmp->next && tmp->next->next) {
         dsc_destroy_buffer(&tmp->next->data);
         _dsc_del_ll_node(tmp->next);
         tmp->next = tmp->next->next;
      } else if (tmp->next) { /* Deleting second to last node */
         dsc_destroy_buffer(&tmp->next->data);
         _dsc_del_ll_node(tmp->next);
         tmp->next = NULL;
      } else { /* Deleting last node in the list */
         if (prev == tmp) { /* Last node is the only node left */
            dsc_destroy_buffer(&tmp->data);
            _dsc_del_ll_node(tmp);
         } else { /* Last node is not the only node left */
            dsc_destroy_buffer(&tmp->data);
            _dsc_del_ll_node(tmp);
            prev->next = NULL;
         }
      }
   } else {
      DSC_ERROR("Tried to free a node that was out of bounds");
      return DSC_EINVAL;
   }

   return DSC_EOK;
}

/* TODO: Add check to see if head contains data */
DSC_DECL ssize_t dsc_get_ll_len(pLLNode_t head) {
   ssize_t i = 1;
   pLLNode_t tmp = head;

   while (tmp->next) {
      tmp = tmp->next;
      i++;
   }

   return i;
}

DSC_DECL pLLNode_t dsc_get_ll_node(pLLNode_t head, const unsigned index) {
   int i = 0;
   pLLNode_t tmp = head;

   while (tmp->next && i < index) {
      tmp = tmp->next;
      i++;
   }

   if (i == index) {
      return tmp;
   } else {
      DSC_ERROR("Tried to access a node that was out of bounds");
      return NULL;
   }
}

DSC_DECL DSC_Error dsc_clear_ll(pLLNode_t head, const uint8_t byte) {
   int status;
   pLLNode_t tmp = head;

   while (tmp->next) {
      status = dsc_clear_buffer(tmp->data, byte);
      if (status != DSC_EOK) {
         DSC_ERROR("Failed to clear data for node");
         return status;
      }
      tmp = tmp->next;
   }

   return DSC_EOK;
}

DSC_DECL DSC_Error dsc_destroy_ll(pLLNode_t head) {
   pLLNode_t tmp = head;
   pLLNode_t prev;

   while (tmp->next) {
      prev = tmp;
      tmp = tmp->next;

      dsc_destroy_buffer(&prev->data);
      _dsc_del_ll_node(prev);
   }

   dsc_destroy_buffer(&tmp->data);
   _dsc_del_ll_node(tmp);

   return DSC_EOK;
}
