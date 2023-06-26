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

#include "../include/buffer.h"

/*
 * ===============================
 *       Public Functions
 * ===============================
 */

/**
 * Creates a new general purpose buffer.
 *
 * @since 04/06/2022
 * @param[in] len The length of the buffer
 * @param[in] tsize The size of the buffer datatype in bytes
 * @returns A pointer to the new buffer object if successful, or NULL otherwise
 */
DSC_DECL pBuffer_t create_buffer(size_t len, size_t tsize) {
   size_t bsize = len * tsize;
   pBuffer_t buf = NULL;
   void *addr = NULL;

   /* Input validation (size_t specifies no upper bound, so we can't check that) */
   if (len <= 0 || tsize <= 0) {
      DSC_ERROR("Invalid parameter");
      return NULL;
   }

   /* Allocate space for the buffer struct */
   buf = mmap(NULL, sizeof(buffer_t), (PROT_READ | PROT_WRITE), (MAP_SHARED | MAP_ANON), -1, 0);
   if (MAP_FAILED == buf) {
      DSC_ERROR("Failed to allocate memory for pBuffer_t");
      return NULL;
   }

   /* Allocate space for the buffer itself */
   addr = mmap(NULL, bsize, (PROT_READ | PROT_WRITE), (MAP_SHARED | MAP_ANON), -1, 0);
   if (MAP_FAILED == addr) {
      DSC_ERROR("Failed to allocate memory for buffer");
      return NULL;
   }

   buf->addr  = addr;
   buf->tsize = tsize;
   buf->bsize = bsize;

   return buf;
}

/**
 * Free an alloated buffer.
 *
 * @since 04/06/2022
 * @param[in/out] bp A pointer to a buffer_t
 * @returns A status code representing the state of operations upon completion
 */
DSC_DECL int free_buffer(pBuffer_t bp) {
   int status;

   if (!bp || !bp->addr) {
      DSC_ERROR("The buffer points to an invalid address");
      return DSC_EFAULT;
   } else {
      /* Deallocate buffer */
      assert((long)(bp->addr) % sysconf(_SC_PAGE_SIZE) == 0);
      status = munmap(bp->addr, bp->bsize);
      if (status != 0) {
         DSC_ERROR("Failed to unmap buffer");
         return DSC_EFREE;
      }

      /* Deallocate buffer struct */ 
      assert((long)bp % sysconf(_SC_PAGE_SIZE) == 0);
      status = munmap(bp, sizeof(buffer_t));
      if (status != 0) {
         DSC_ERROR("Failed to unmap pBuffer_t struct");
         return DSC_EFREE;
      }

      bp = NULL;
   }

   return DSC_EOK;
}

/**
 * Resize an existing buffer.
 *
 * @since 04/06/2022
 * @param[in] newlen The new length of the buffer
 * @param[in/out] bp A pointer to a buffer_t
 * @returns A status code representing the state of operations upon completion
 */
DSC_DECL int resize_buffer(pBuffer_t bp, size_t newlen) {
   if (newlen <= 0) {
      DSC_ERROR("The new buffer length is invalid");
      return DSC_EINVAL;
   } else if (!bp->addr) {
      DSC_ERROR("The buffer points to an invalid address");
      return DSC_EFAULT;
   } else {
      /* TODO: mremap is not portable outside of Linux!!! */
      assert((long)(bp->addr) % sysconf(_SC_PAGE_SIZE) == 0);
      bp->addr = mremap(bp->addr, bp->bsize, (newlen * bp->tsize), 0);
      if (MAP_FAILED == bp->addr) {
         DSC_ERROR("Failed to remap buffer");
         return DSC_ERROR;
      }
   }

   bp->bsize = newlen * bp->tsize;
   return DSC_EOK;
}

/**
 * Clears the buffer by filling it with "byte".
 *
 * @since 04/06/2022
 * @param[in] byte The byte to fill the buffer with
 * @param[in/out] bp A pointer to a buffer_t
 * @returns A status code representing the state of operations upon completion
 */
DSC_DECL int clear_buffer(pBuffer_t bp, uint8_t byte) {
   if (!bp->addr) {
      DSC_ERROR("The buffer points to an invalid address");
      return DSC_EFAULT;
   }

   memset(bp->addr, (int)byte, bp->bsize);
   return DSC_EOK;
}

/**
 * Returns the size of the buffer in bytes.
 *
 * @since 04/06/2022
 * @param[in/out] bp A pointer to a buffer_t
 * @returns The size of the buffer in bytes or -1 upon failure
 */
DSC_DECL ssize_t buffer_size(pBuffer_t bp) {
   if (!bp->addr) {
      DSC_ERROR("The buffer points to an invalid address");
      return -1;
   }

   return bp->bsize;
}

/**
 * Returns the number of elements of size tsize that a buffer can fit.
 *
 * @since 04/06/2022
 * @param[in/out] bp A pointer to a buffer_t
 * @returns The length of the buffer or -1 upon failure
 */
DSC_DECL ssize_t buffer_len(pBuffer_t bp) {
   if (!bp->addr) {
      DSC_ERROR("The buffer points to an invalid address");
      return -1;
   }

   return (bp->bsize / bp->tsize);
}
