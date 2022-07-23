/**
 * @file buffer.c
 * This source file provides basic functionality for
 * creating a generic buffer. It is used by all other
 * data structures, and is thus the cornerstone of the
 * library.
 *
 * **Author:** Neil Kingdom
 * **Version:** 1.0
 * **Since:** 04-06-2022
*/
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stddef.h>
#include <sys/mman.h>
#include "buffer.h"

/**
 * @param[in] len The length of the buffer
 * @param[in] bsize The size of the buffer datatype in bytes
 */
DSC_DECL void *create_buffer(size_t len, size_t bsize)
{
   void *buffer = mmap(NULL, len * bsize, PROT_READ | PROT_WRITE, MAP_SHARED, 0, 0);
   if (MAP_FAILED == buffer)
   {
      DSC_ERROR(__LINE__, __FUNCTION__, __FILE__);
      fprintf(stderr, "Failed to allocate memory for buffer: %s\n", strerror(errno));
      return NULL;
   }

   return buffer;
}

DSC_DECL int free_buffer(size_t len, size_t bsize, void *buffer)
{
   int status;

   if (!buffer)
   {
      DSC_ERROR(__LINE__, __FUNCTION__, __FILE__);
      fprintf(stderr, "Cannot free buffer twice\n");
      return DSC_EFREE;
   }
   else
   {
      status = munmap(buffer, len * bsize);
      if (status != 0)
      {
         DSC_ERROR(__LINE__, __FUNCTION__, __FILE__);
         fprintf(stderr, "Failed to unmap buffer: %s\n", strerror(errno));
         return DSC_EFREE;
      }
   }

   return DSC_EOK;
}

DSC_DECL int resize_buffer(size_t oldlen, size_t newlen, size_t bsize, void *buffer)
{
   int status;

   if (!buffer)
   {
      DSC_ERROR(__LINE__, __FUNCTION__, __FILE__);
      fprintf(stderr, "Cannot free buffer twice\n");
      return DSC_EFREE;
   }
   else
   {
      /* According to mremap, address must be page aligned */
      assert(buffer % sysconf(_SC_PAGE_SIZE) == 0);
      status = mremap(buffer, oldlen * bsize, newlen * bsize, 0);
      if (MAP_FAILED == status)
      {
         DSC_ERROR(__LINE__, __FUNCTION__, __FILE__);
         fprintf(stderr, "Failed to unmap buffer: %s\n", strerror(errno));
         return DSC_ERROR;
      }
   }

   return DSC_EOK;
}

DSC_DECL int reset_buffer(size_t len, size_t bsize, void *buffer)
{
   if (!buffer)
   {
      DSC_ERROR(__LINE__, __FUNCTION__, __FILE__);
      fprintf(stderr, "Cannot free buffer twice\n");
      return DSC_EFREE;
   }

   memset(buffer, 0, len * bsize);

   return DSC_EOK;
}

DSC_DECL size_t sizeof_buffer(void *buffer)
{
   if (!buffer)
   {
      DSC_ERROR(__LINE__, __FUNCTION__, __FILE__);
      fprintf(stderr, "Cannot free buffer twice\n");
      return DSC_EFREE;
   }

   /* TODO: Not sure if there is a better way to calculate length */
   return (strlen((char *)buffer));
}
