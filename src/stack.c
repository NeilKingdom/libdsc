/**
 * This source file provides basic functionality for
 * creating a generic buffer. It is used by all other
 * data structures, and is thus the cornerstone of the
 * library.
 *
 * @file
 * @author Neil Kingdom
 * @version 1.0
 * @since 04/06/2022
*/
#define _GNU_SOURCE
#include <unistd.h>
#include <assert.h>
#include <sys/mman.h>

#include "../include/stack.h"

/*
 * ===============================
 *       Public Functions
 * ===============================
 */

DSC_DECL pStack_t create_stack(void) {
   pStack_t stack = NULL;
   pBuffer_t init_node = NULL;

   /* Allocate space for the stack */
   stack = mmap(NULL, sizeof(stack_t), (PROT_READ | PROT_WRITE), (MAP_SHARED | MAP_ANON), -1, 0);
   if (MAP_FAILED == stack) {
      DSC_ERROR("Failed to allocate memory for pStack_t");
      return NULL;
   }

   /* Initialize members to 0/NULL */
   memset(stack, 0, sizeof(stack_t));

   return stack;
}

DSC_DECL int push(pStack_t stack, size_t len, size_t tsize, void *data) {
   size_t ssize = stack->stk_off + sizeof(buffer_t);  
   size_t new_size = ssize + sizeof(buffer_t);

   /* Check if stacks been initialized */
   if (!stack->base_ptr) {
      stack->base_ptr = mmap(NULL, sizeof(buffer_t), (PROT_READ | PROT_WRITE), (MAP_SHARED | MAP_ANON), -1, 0);
      if (MAP_FAILED == stack->base_ptr) {
         DSC_ERROR("Failed to allocate memory for the stack's initial node");
         return DSC_ERROR;
      }
   } else { /* Resize stack to fit new data */
      assert((long)(stack->base_ptr) % sysconf(_SC_PAGE_SIZE) == 0);
      stack->base_ptr = mremap(stack->base_ptr, ssize, new_size, 0);
      if (MAP_FAILED == stack->base_ptr) {
         DSC_ERROR("Failed to remap stack");
         return DSC_ERROR;
      }

      stack->stk_off += sizeof(buffer_t);
   }

   /* Create pBuffer_t to contain data */
   pBuffer_t *curr_node = stack->base_ptr + stack->stk_off;
   *curr_node = create_buffer(len, tsize);
   if (*curr_node == NULL) {
      DSC_ERROR("Failed to allocate memory for the stack's initial node");
      return DSC_ERROR;
   }

   memmove((*curr_node)->addr, data, buffer_size(*curr_node));

   return DSC_EOK;
}

DSC_DECL pBuffer_t pop(pStack_t stack) {
   size_t ssize = stack->stk_off + sizeof(buffer_t); 
   size_t new_size = ssize - sizeof(buffer_t);

   if (!stack->base_ptr) {
      DSC_ERROR("The stack points to an invalid address");
      return NULL;
   }

   free_buffer(*(stack->base_ptr + stack->stk_off));

   /* Truncate stack length */
   if (new_size > 0) {
      assert((long)(stack->base_ptr) % sysconf(_SC_PAGE_SIZE) == 0);
      stack->base_ptr = mremap(stack->base_ptr, ssize, new_size, 0);
      if (MAP_FAILED == stack->base_ptr) {
         DSC_ERROR("Failed to remap stack");
         return NULL;
      }

      stack->stk_off -= sizeof(buffer_t);
   } else {
      munmap(stack->base_ptr, ssize);
      stack->base_ptr = NULL;
   }

   /* TODO: Return copy of node that was popped? */

   return NULL;
}

DSC_DECL pBuffer_t peek(pStack_t stack) {
   if (!stack->base_ptr) {
      DSC_ERROR("The stack points to an invalid address");
      return NULL;
   }

   return *(stack->base_ptr + stack->stk_off);
}
