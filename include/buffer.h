#ifndef BUFFER_H
#define BUFFER_H

#include "dsc_common.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct {
   void  *addr;  /* Base address of the memory region */
   size_t tsize; /* The size (in bytes) of the data type used for the buffer's memory region */
   size_t bsize; /* The size (in bytes) of the buffer's memory region */
} Buffer_t;

/* Buffer type-cast macro helper functions */

#define DSC_BUF_AS_CHAR(buf)        ((char*)((buf).addr))
#define DSC_BUF_AS_UCHAR(buf)       ((unsigned char*)((buf).addr))
#define DSC_BUF_AS_SHORT(buf)       ((short*)((buf).addr))
#define DSC_BUF_AS_USHORT(buf)      ((unsigned short*)((buf).addr))
#define DSC_BUF_AS_INT(buf)         ((int*)((buf).addr))
#define DSC_BUF_AS_UINT(buf)        ((unsigned int*)((buf).addr))
#define DSC_BUF_AS_LONG(buf)        ((long*)((buf).addr))
#define DSC_BUF_AS_ULONG(buf)       ((unsigned long*)((buf).addr))
#define DSC_BUF_AS_LONGLONG(buf)    ((long long*)((buf).addr))
#define DSC_BUF_AS_ULONGLONG(buf)   ((unsigned long long*)((buf).addr))
#define DSC_BUF_AS_SSIZE_T(buf)     ((ssize_t*)((buf).addr))
#define DSC_BUF_AS_SIZE_T(buf)      ((size_t*)((buf).addr))
#define DSC_BUF_AS_FLOAT(buf)       ((float*)((buf).addr))
#define DSC_BUF_AS_DOUBLE(buf)      ((double*)((buf).addr))
#define DSC_BUF_AS_INT8(buf)        ((int8_t*)((buf).addr))
#define DSC_BUF_AS_UINT8(buf)       ((uint8_t*)((buf).addr))
#define DSC_BUF_AS_INT16(buf)       ((int16_t*)((buf).addr))
#define DSC_BUF_AS_UINT16(buf)      ((uint16_t*)((buf).addr))
#define DSC_BUF_AS_INT32(buf)       ((int32_t*)((buf).addr))
#define DSC_BUF_AS_UINT32(buf)      ((uint32_t*)((buf).addr))
#define DSC_BUF_AS_INT64(buf)       ((int64_t*)((buf).addr))
#define DSC_BUF_AS_UINT64(buf)      ((uint64_t*)((buf).addr))

/* Forward function declarations */

DSC_DECL Buffer_t       dsc_buf_create(const size_t nelem, const size_t tsize);
DSC_DECL DscError_t     dsc_buf_destroy(Buffer_t *buf);
DSC_DECL DscError_t     dsc_buf_resize(Buffer_t *buf, const size_t nelem);
DSC_DECL DscError_t     dsc_buf_fill(Buffer_t *buf, const uint8_t byte);
DSC_DECL ssize_t        dsc_buf_capacity(const Buffer_t buf);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* BUFFER_H */
