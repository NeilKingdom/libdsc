#ifndef COMMON_H
#define COMMON_H

#define _GNU_SOURCE /* This must be defined prior to #including sys/mman.h in order to use mremap */
#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stddef.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>

/* Change to static if you want function declarations to be static */
#define DSC_DECL

#define DSC_ERROR(file, func, line) printf("Error: File %s, Function %s, Line %d\n",\
                                    (file), (func), (line))

enum DSC_Error
{
   DSC_EOK = 0, /* No error */
   DSC_ERROR,   /* General error */
   DSC_EINVAL,  /* The argument was invalid */
   DSC_ENOMEM,  /* Not enough memory */
   DSC_EFREE,   /* Error freeing memory */
   DSC_EFAULT   /* Bad address */
};

/*
   Make sure each member of the node union is same size (sizeof(pointer) * 3)
   Struct declarations are defined internally so that they cannot be created elsewhere
 */
/*
typedef union node
{
   / Node with next ref. /
   struct ndn
   {
      void *data;
      ndn  *next;
      void *padding;
   };
   / Node with next ref. and prev ref. /
   struct ndnp
   {
      void *data;
      ndnp *next;
      ndnp *prev;
   };
} *pHead_t, head_t,
  *pTail_t, tail_t;
*/

#endif
