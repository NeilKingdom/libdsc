/**
 * Example Usage:
 *
 * #include "buffer.h"
 *
 * #define BUF_TYPE int
 * #define TSIZE sizeof(BUF_TYPE)
 * #define BLEN (size_t)10
 *
 * int main(void)
 * {
 *    pBuffer_t buf;
 *    const char *test_msg = "Hello World!";
 *
 *    buf = create_buffer(BLEN, TSIZE);
 *    printf("The length of buffer buf is: %ld\n", buffer_len(buf));
 *
 *    resize_buffer(20, buf);
 *    printf("The new length of buffer buf is: %ld\n", buffer_len(buf));
 *
 *    memcpy(DSC_BUF_VOID(buf), (void *)test_msg, strlen(test_msg) + 1);
 *    printf("Buffer says: %s\n", DSC_BUF_CHAR(buf));
 *
 *    free_buffer(buf);
 *    if (!DSC_BUF_VOID(buf))
 *       printf("Successfully freed buffer!\n");
 *
 *    return 0;
 * }
 */

#ifndef BUFFER_H
#define BUFFER_H

#include "common.h"

typedef struct buffer
{
   void *addr;   /* Address of the memory arena */
   size_t tsize; /* The datatype size */
   size_t bsize; /* The buffer size */
} *pBuffer_t, buffer_t;

/* Buffer type cast macro helper functions */
#define DSC_BUF_VOID(buf)        ((void *)(buf->addr))
#define DSC_BUF_CHAR(buf)        ((char *)(buf->addr))
#define DSC_BUF_UCHAR(buf)       ((unsigned char *)(buf->addr))
#define DSC_BUF_SHORT(buf)       ((short *)(buf->addr))
#define DSC_BUF_USHORT(buf)      ((unsigned short *)(buf->addr))
#define DSC_BUF_INT(buf)         ((int *)(buf->addr))
#define DSC_BUF_UINT (buf)       ((unsigned *)(buf->addr))
#define DSC_BUF_LONG(buf)        ((long *)(buf->addr))
#define DSC_BUF_ULONG(buf)       ((unsigned long *)(buf->addr))
#define DSC_BUF_LONG_LONG(buf)   ((long long *)(buf->addr))
#define DSC_BUF_SIZE_T(buf)      ((size_t *)(buf->addr))
#define DSC_BUF_FLOAT(buf)       ((float *)(buf->addr))
#define DSC_BUF_DOUBLE(buf)      ((double *)(buf->addr))

#define DSB_BUF_INT8(buf)        ((int8_t *)(buf->addr))
#define DSB_BUF_UINT8(buf)       ((uint8_t *)(buf->addr))
#define DSB_BUF_INT16(buf)       ((uint16_t *)(buf->addr))
#define DSB_BUF_UINT16(buf)      ((uint16_t *)(buf->addr))
#define DSB_BUF_INT32(buf)       ((int32_t *)(buf->addr))
#define DSB_BUF_UINT32(buf)      ((uint32_t *)(buf->addr))
#define DSB_BUF_INT64(buf)       ((int64_t *)(buf->addr))
#define DSB_BUF_UINT64(buf)      ((uint64_t *)(buf->addr))

/* Forward function declarations */
DSC_DECL pBuffer_t   create_buffer(size_t len, size_t tsize);
DSC_DECL int         free_buffer(pBuffer_t bp);
DSC_DECL int         resize_buffer(size_t newlen, pBuffer_t bp);
DSC_DECL int         reset_buffer(unsigned char byte, pBuffer_t bp);
DSC_DECL size_t      buffer_size(pBuffer_t bp);
DSC_DECL size_t      buffer_len(pBuffer_t bp);
DSC_DECL int         destroy_buffer(pBuffer_t bp);

#endif
