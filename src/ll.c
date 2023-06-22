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
 * Free an allocated pNode_t struct.
 *
 * @since 06/22/2023
 * @param [in] np A pointer to a node_t
 * @returns A status code representing the state of operations upon completion
 */
static int _free_ll_node(pNode_t np) {
   int status;

   if (!np) {
      DSC_ERROR("The node points to an invalid address");
      return DSC_EFAULT;
   } else {
      assert((long)np % sysconf(_SC_PAGE_SIZE) == 0);
      status = munmap(np, sizeof(node_t));
      if (status != 0) {
         DSC_ERROR("Failed to unmap pNode_t struct");
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

DSC_DECL pNode_t create_ll(size_t len, size_t tsize, void *data) {
   pNode_t head = NULL;

   /* Allocate space for the node struct */
   head = mmap(NULL, sizeof(node_t), (PROT_READ | PROT_WRITE), (MAP_SHARED | MAP_ANON), -1, 0);
   if (MAP_FAILED == head) {
      DSC_ERROR("Failed to allocate memory for pNode_t");
      return NULL;
   }

   head->next = NULL;
   head->data = create_buffer(len, tsize);
   if (head->data == NULL) {
      DSC_ERROR("Failed to allocate memory for the node's data");
      return NULL;
   }

   return head;
}

DSC_DECL int add_ll_node(pNode_t head, size_t len, size_t tsize, void *data) {
   size_t nsize = len * tsize;
   pNode_t new_node = NULL;
   pNode_t tmp = head;

   while (tmp->next) {
      tmp = tmp->next;
   }

   /* Allocate space for the node struct */
   new_node = mmap(NULL, sizeof(node_t), (PROT_READ | PROT_WRITE), (MAP_SHARED | MAP_ANON), -1, 0);
   if (MAP_FAILED == new_node) {
      DSC_ERROR("Failed to allocate memory for pNode_t");
      return DSC_ERROR;
   }

   /* Allocate space for the node's data */
   new_node->next = NULL;
   new_node->data = create_buffer(len, tsize);
   if (new_node->data == NULL) {
      DSC_ERROR("Failed to allocate memory for the node's data");
      return DSC_ERROR;
   }

   /* Copy data passed by user to the memory segment */
   /* TODO: nsize may be bigger than memmove allows */
   memmove(new_node->data->addr, data, nsize);

   tmp->next = new_node;

   return DSC_EOK;
}

DSC_DECL int free_ll_node(pNode_t head, unsigned index) {
   int i = 0;
   pNode_t tmp = head;
   pNode_t prev = tmp; /* Keep track of previous node in case we need to delete last node */

   while (tmp->next && i++ < index) {
      prev = tmp;
      tmp = tmp->next;
   }

   if (i == index) {
      if (tmp->next && tmp->next->next) {
         free_buffer(tmp->next->data); 
         _free_ll_node(tmp->next);     
         tmp->next = tmp->next->next;
      } else if (tmp->next) { /* Deleting second to last node */
         free_buffer(tmp->next->data);
         _free_ll_node(tmp->next);
         tmp->next = NULL;
      } else { /* Deleting last node in the list */
         if (prev == tmp) { /* Last node is the only node left */
            free_buffer(tmp->data);
            _free_ll_node(tmp);
         } else { /* Last node is not the only node left */
            free_buffer(tmp->data);
            _free_ll_node(tmp);
            prev->next = NULL;
         }
      }
   } else {
      DSC_ERROR("Tried to free a node that was out of bounds");
      return DSC_EINVAL;
   }

   return DSC_EOK;
}

DSC_DECL ssize_t get_ll_len(pNode_t head) {
   ssize_t i = 1;
   pNode_t tmp = head;

   while (tmp->next) {
      tmp = tmp->next;
      i++;
   }

   return i;
}

DSC_DECL pNode_t get_ll_node(pNode_t head, unsigned index) {
   int i = 0;
   pNode_t tmp = head;

   while (tmp->next && i++ < index) {
      tmp = tmp->next;
   }

   if (i == index) {
      return tmp;
   } else {
      DSC_ERROR("Tried to access a node that was out of bounds");
      return NULL;
   }
}

DSC_DECL int clear_ll_data(pNode_t head, uint8_t byte) {
   int status;
   pNode_t tmp = head;

   while (tmp->next) {
      status = clear_buffer(tmp->data, byte);
      if (status != DSC_EOK) {
         DSC_ERROR("Failed to clear data for node");
         return status;
      }
      tmp = tmp->next;
   }

   return DSC_EOK;
}