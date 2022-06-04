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
#include <stddef.h>
#include "common.h"

/* TODO: Better to use mmap rather than malloc() */
void *create_buffer(size_t size)
{
   void *buffer = malloc(size);
   if (buffer == NULL)
   {
      fprintf(stderr, "Failed to allocate memory for buffer: %s\n", strerror(errno));
      DSC_ERROR(__LINE__, __FUNCTION__, __FILE__);
      return NULL;
   }

   return buffer;
}

int free_buffer(void *buffer)
{
   if (!buffer)
   {
      fprintf(stderr, "Cannot free memory object twice.\n");
      DSC_ERROR(__LINE__, __FUNCTION__, __FILE__);
      return DSC_EDFREE;
   }
   else
   {
      free(buffer);
   }

   return 0;
}


