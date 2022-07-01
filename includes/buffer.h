#ifndef BUFFER_H
#define BUFFER_H

#include "common.h"

#define INIT_BUFFER(b)        (b) = { .begin = NULL, .type = DSC_BUFFER }

typedef struct buffer
{
   void *begin;
   int type;
} *pBuffer_t, buffer_t;

DSC_DECL void *create_buffer(size_t len, size_t bsize);
DSC_DECL int free_buffer(size_t len, size_t bsize, void *buffer);
DSC_DECL int resize_buffer(size_t oldlen, size_t newlen, size_t bsize, void *buffer);
DSC_DECL int reset_buffer(size_t len, size_t bsize, void *buffer);
DSC_DECL size_t sizeof_buffer(void *buffer);

#endif
