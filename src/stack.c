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
   size_t old_size, new_size;
   pBuffer_t new_node;

   /* Allocate space for intial node if not done */
   if (!stack->base_ptr) {
      stack->base_ptr = mmap(NULL, sizeof(buffer_t), (PROT_READ | PROT_WRITE), (MAP_ANON | MAP_SHARED), -1, 0);
      if (MAP_FAILED == stack->base_ptr) {
         DSC_ERROR("Failed to allocate space for the stack");
         return DSC_ERROR;
      }
   } else { /* Resize stack */
      old_size = stack->stk_off + sizeof(buffer_t);
      new_size = old_size + sizeof(buffer_t);
      stack->base_ptr = mremap(stack->base_ptr, old_size, new_size, 0);
      if (MAP_FAILED == stack->base_ptr) {
         DSC_ERROR("Failed to remap the stack");
         return DSC_ERROR;
      }

      if (stack->pending) {
         destroy_buffer(stack->pending);
         stack->pending = NULL;
      } else {
         stack->stk_off += sizeof(buffer_t);
      }
   }

   /* Point current node to new buffer */
   new_node = create_buffer(len, tsize);
   if (new_node == NULL) {
      DSC_ERROR("Failed to allocate space for new stack node");
      return DSC_ERROR;
   }

   /* Put data into new buffer */
   memmove(new_node->addr, data, buffer_size(new_node));
   *(stack->base_ptr + stack->stk_off) = new_node; 

   return DSC_EOK;
}

DSC_DECL pBuffer_t pop(pStack_t stack) {
   size_t old_size, new_size;
   pBuffer_t curr_node, prev_node;
   
   /* Fail if stack has not been allocated */
   if (!stack->base_ptr) {
      DSC_ERROR("The stack points to an invalid address");
      return NULL;
   }
  
   /* Destroy pending nodes */
   if (stack->pending) {
      destroy_buffer(stack->pending);
      stack->pending = NULL;

      old_size = stack->stk_off + sizeof(buffer_t);
      new_size = old_size - sizeof(buffer_t);
   
      /* unmap if last node on the stack */
      if (new_size == 0) {
         munmap(stack->base_ptr, sizeof(buffer_t));
         stack->base_ptr = NULL;
         /* TODO: Error handling */
      } else { /* Truncate to the appropriate length */
         stack->base_ptr = mremap(stack->base_ptr, old_size, new_size, 0);
         if (MAP_FAILED == stack->base_ptr) {
            DSC_ERROR("Failed to remap the stack");
            return NULL;
         }
      }

      if (stack->stk_off > 0) {
         stack->stk_off -= sizeof(buffer_t);
      }
   } 

   /* Can be NULL if unmapped from popping last node */
   if (stack->base_ptr + stack->stk_off) {
      stack->pending = *(stack->base_ptr + stack->stk_off);
   }

   return stack->pending;
}

DSC_DECL pBuffer_t peek(pStack_t stack) {
   if (!stack->base_ptr) {
      DSC_ERROR("The stack points to an invalid address");
      return NULL;
   }

   return *(stack->base_ptr + stack->stk_off);
}

DSC_DECL int destroy_stack(pStack_t stack) {
   int status; 

   /* Fail if stack has not been allocated */
   if (!stack || !stack->base_ptr) {
      DSC_ERROR("The stack points to an invalid address");
      return DSC_ERROR;
   } else if (stack) {
      /* Unmap the array of pointers to each node in the stack */
      status = munmap(stack->base_ptr, (stack->stk_off + sizeof(buffer_t)));
      if (status == -1) {
         DSC_ERROR("Failed to unmap the stack");
         return DSC_EFREE;
      }

      /* Unmap the stack struct itself */
      status = munmap(stack, sizeof(stack_t));
      if (status == -1) {
         DSC_ERROR("Failed to unmap the pStack_t struct");
         return DSC_EFREE;
      }
   }

   return DSC_EOK;
}
