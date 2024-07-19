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
} *pBuffer_t, Buffer_t;

/* Buffer type cast macro helper functions */
#define DSC_BUF_VOID(buf)        ((void *)((buf).addr))
#define DSC_BUF_CHAR(buf)        ((char *)((buf).addr))
#define DSC_BUF_UCHAR(buf)       ((unsigned char *)((buf).addr))
#define DSC_BUF_SHORT(buf)       ((short *)((buf).addr))
#define DSC_BUF_USHORT(buf)      ((unsigned short *)((buf).addr))
#define DSC_BUF_INT(buf)         ((int *)((buf).addr))
#define DSC_BUF_UINT(buf)        ((unsigned *)((buf).addr))
#define DSC_BUF_LONG(buf)        ((long *)((buf).addr))
#define DSC_BUF_ULONG(buf)       ((unsigned long *)((buf).addr))
#define DSC_BUF_LONG_LONG(buf)   ((long long *)((buf).addr))
#define DSC_BUF_SIZE_T(buf)      ((size_t *)((buf).addr))
#define DSC_BUF_SSIZE_T(buf)     ((ssize_t *)((buf).addr))
#define DSC_BUF_FLOAT(buf)       ((float *)((buf).addr))
#define DSC_BUF_DOUBLE(buf)      ((double *)((buf).addr))

#define DSC_BUF_INT8(buf)        ((int8_t *)((buf).addr))
#define DSC_BUF_UINT8(buf)       ((uint8_t *)((buf).addr))
#define DSC_BUF_INT16(buf)       ((int16_t *)((buf).addr))
#define DSC_BUF_UINT16(buf)      ((uint16_t *)((buf).addr))
#define DSC_BUF_INT32(buf)       ((int32_t *)((buf).addr))
#define DSC_BUF_UINT32(buf)      ((uint32_t *)((buf).addr))
#define DSC_BUF_INT64(buf)       ((int64_t *)((buf).addr))
#define DSC_BUF_UINT64(buf)      ((uint64_t *)((buf).addr))

#define DSC_PBUF_VOID(buf)       ((void *)((buf)->addr))
#define DSC_PBUF_CHAR(buf)       ((char *)((buf)->addr))
#define DSC_PBUF_UCHAR(buf)      ((unsigned char *)((buf)->addr))
#define DSC_PBUF_SHORT(buf)      ((short *)((buf)->addr))
#define DSC_PBUF_USHORT(buf)     ((unsigned short *)((buf)->addr))
#define DSC_PBUF_INT(buf)        ((int *)((buf)->addr))
#define DSC_PBUF_UINT(buf)       ((unsigned *)((buf)->addr))
#define DSC_PBUF_LONG(buf)       ((long *)((buf)->addr))
#define DSC_PBUF_ULONG(buf)      ((unsigned long *)((buf)->addr))
#define DSC_PBUF_LONG_LONG(buf)  ((long long *)((buf)->addr))
#define DSC_PBUF_SIZE_T(buf)     ((size_t *)((buf)->addr))
#define DSC_PBUF_SSIZE_T(buf)    ((ssize_t *)((buf)->addr))
#define DSC_PBUF_FLOAT(buf)      ((float *)((buf)->addr))
#define DSC_PBUF_DOUBLE(buf)     ((double *)((buf)->addr))

#define DSC_PBUF_INT8(buf)       ((int8_t *)((buf)->addr))
#define DSC_PBUF_UINT8(buf)      ((uint8_t *)((buf)->addr))
#define DSC_PBUF_INT16(buf)      ((int16_t *)((buf)->addr))
#define DSC_PBUF_UINT16(buf)     ((uint16_t *)((buf)->addr))
#define DSC_PBUF_INT32(buf)      ((int32_t *)((buf)->addr))
#define DSC_PBUF_UINT32(buf)     ((uint32_t *)((buf)->addr))
#define DSC_PBUF_INT64(buf)      ((int64_t *)((buf)->addr))
#define DSC_PBUF_UINT64(buf)     ((uint64_t *)((buf)->addr))

/* Forward function declarations */

DSC_DECL Buffer_t       dsc_create_buffer(const size_t len, const size_t tsize);
DSC_DECL DscError_t     dsc_destroy_buffer(pBuffer_t buf);
DSC_DECL DscError_t     dsc_resize_buffer(pBuffer_t buf, const size_t newlen);
DSC_DECL DscError_t     dsc_clear_buffer(Buffer_t buf, const uint8_t byte);
DSC_DECL ssize_t        dsc_get_buffer_capacity(const Buffer_t buf);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* BUFFER_H */
