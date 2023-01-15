/**
 * This source file provides basic functionality for
 * creating a generic buffer. It is used by all other
 * data structures, and is thus the cornerstone of the
 * library.
 *
 * @file
 * @author: Neil Kingdom
 * @version: 1.0
 * @since: 04-06-2022
*/
#include "buffer.h"

/**
 * Creates a new general purpose buffer
 *
 * @param[in] len The length of the buffer
 * @param[in] tsize The size of the buffer datatype in bytes
 * @return A pointer to the new buffer object if successful, or NULL otherwise
 */
DSC_DECL pBuffer_t create_buffer(size_t len, size_t tsize)
{
   size_t bsize = len * tsize;
   pBuffer_t buf = NULL;
   void *addr = NULL;

   /* Input validation (size_t specifies no upper bound, so we can't check that) */
   if (len <= 0 || tsize <= 0)
   {
      DSC_ERROR(__FILE__, __func__, __LINE__);
      fprintf(stderr, "Invalid parameter\n");
      return NULL;
   }

   /* Allocate space for the buffer struct */
   buf = mmap(NULL, sizeof(buffer_t), (PROT_READ | PROT_WRITE), (MAP_SHARED | MAP_ANON), -1, 0);
   if (MAP_FAILED == buf)
   {
      DSC_ERROR(__FILE__, __func__, __LINE__);
      fprintf(stderr, "Failed to allocate memory for pBuffer_t: %s\n", strerror(errno));
      return NULL;
   }

   /* Allocate space for the buffer itself */
   addr = mmap(NULL, bsize, (PROT_READ | PROT_WRITE), (MAP_SHARED | MAP_ANON), -1, 0);
   if (MAP_FAILED == addr)
   {
      DSC_ERROR(__FILE__, __func__, __LINE__);
      fprintf(stderr, "Failed to allocate memory for buffer: %s\n", strerror(errno));
      return NULL;
   }

   buf->addr = addr;
   buf->tsize = tsize;
   buf->bsize = bsize;

   return buf;
}

/**
 * Free an allocated buffer
 *
 * @param[in/out] bp A pointer to a buffer_t
 * @return A status code representing the state of operations upon completion
 */
DSC_DECL int free_buffer(pBuffer_t bp)
{
   int status;

   if (!bp->addr)
   {
      DSC_ERROR(__FILE__, __func__, __LINE__);
      fprintf(stderr, "The buffer points to an invalid address\n");
      return DSC_EFAULT;
   }
   else
   {
      /* According to munmap, address must be page aligned */
      assert((long)(bp->addr) % sysconf(_SC_PAGE_SIZE) == 0);
      status = munmap(bp->addr, bp->bsize);
      if (status != 0)
      {
         DSC_ERROR(__FILE__, __func__, __LINE__);
         fprintf(stderr, "Failed to unmap buffer: %s\n", strerror(errno));
         return DSC_EFREE;
      }
   }

   bp->addr = NULL;
   return DSC_EOK;
}

/**
 * Resize an existing buffer
 *
 * @param[in] newlen The new length of the buffer
 * @param[in/out] bp A pointer to a buffer_t
 * @return A status code representing the state of operations upon completion
 */
DSC_DECL int resize_buffer(size_t newlen, pBuffer_t bp)
{
   if (newlen <= 0)
   {
      DSC_ERROR(__FILE__, __func__, __LINE__);
      return DSC_EINVAL;
   }
   else if (!bp->addr)
   {
      DSC_ERROR(__FILE__, __func__, __LINE__);
      fprintf(stderr, "The buffer points to an invalid address\n");
      return DSC_EFAULT;
   }
   else
   {
      /* NOTE: mremap is not portable outside of Linux!!! */
      /* According to mremap, old address must be page aligned */
      assert((long)(bp->addr) % sysconf(_SC_PAGE_SIZE) == 0);
      bp->addr = mremap(bp->addr, bp->bsize, (newlen * bp->tsize), 0);
      if (MAP_FAILED == bp->addr)
      {
         DSC_ERROR(__FILE__, __func__, __LINE__);
         fprintf(stderr, "Failed to remap buffer: %s\n", strerror(errno));
         return DSC_ERROR;
      }
   }

   bp->bsize = newlen * bp->tsize;
   return DSC_EOK;
}

/**
 * Clears the buffer by filling it with "byte"
 *
 * @param[in] byte The byte to fill the buffer with
 * @param[in/out] bp A pointer to a buffer_t
 * @return A status code representing the state of operations upon completion
 */
DSC_DECL int clear_buffer(unsigned char byte, pBuffer_t bp)
{
   if (!bp->addr)
   {
      DSC_ERROR(__FILE__, __func__, __LINE__);
      fprintf(stderr, "The buffer points to an invalid address\n");
      return DSC_EFAULT;
   }

   memset(bp->addr, (int)(byte), bp->bsize);
   return DSC_EOK;
}

/**
 * Returns the size of the buffer in bytes
 *
 * @param[in/out] bp A pointer to a buffer_t
 * @return The size of the buffer in bytes or 0 upon failure
 */
DSC_DECL size_t buffer_size(pBuffer_t bp)
{
   if (!bp->addr)
   {
      DSC_ERROR(__FILE__, __func__, __LINE__);
      fprintf(stderr, "The buffer points to an invalid address\n");
      return 0;
   }

   return bp->bsize;
}

/**
 * Returns the number of indices that the buffer contains i.e. its length
 *
 * @param[in/out] bp A pointer to a buffer_t
 * @return The length of the buffer (in terms of indices) or 0 upon failure
 */
DSC_DECL size_t buffer_len(pBuffer_t bp)
{
   if (!bp->addr)
   {
      DSC_ERROR(__FILE__, __func__, __LINE__);
      fprintf(stderr, "The buffer points to an invalid address\n");
      return 0;
   }

   return (bp->bsize / bp->tsize);
}
