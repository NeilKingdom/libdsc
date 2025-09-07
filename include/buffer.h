#ifndef BUFFER_H
#define BUFFER_H

#include "dsc_common.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct {
   void   *base;  // Base address of the memory region
   uint8_t tsize; // The size (in bytes) of the data type used for the buffer's memory region
   size_t  bsize; // The size (in bytes) of the buffer's memory region
} Buffer_t;

// Forward function declarations

DscError_t     dsc_buf_init(Buffer_t *buf, const size_t nelem, const uint8_t tsize);
DscError_t     dsc_buf_resize(Buffer_t *buf, const size_t nelem);
DscError_t     dsc_buf_fill(Buffer_t *buf, const uint8_t byte);
size_t         dsc_buf_nelem(const Buffer_t* const buf);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // BUFFER_H
