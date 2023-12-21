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
DSC_DECL buffer_t dsc_create_buffer(const size_t len, const size_t tsize) {
    void *addr = NULL;
    size_t bsize = len * tsize;
    buffer_t buf = { addr, tsize, bsize };

    /* Input validation (size_t specifies no upper bound, so we can't check that) */
    if (len <= 0 || tsize <= 0) {
        DSC_ERROR("Invalid parameter");
        return buf;
    }

    /* Allocate space for the buffer */
    buf.addr = mmap(NULL, bsize, (PROT_READ | PROT_WRITE), (MAP_SHARED | MAP_ANON), -1, 0);
    if (MAP_FAILED == buf.addr) {
        DSC_ERROR("Failed to allocate memory for buffer");
        buf.addr = NULL;
    }

    return buf;
}

/**
 * Free an alloated buffer.
 *
 * @since 04/06/2022
 * @param[in/out] bp A pointer to a buffer_t
 * @returns A status code representing the state of operations upon completion
 */
DSC_DECL DSC_Error dsc_destroy_buffer(restrict pBuffer_t buf) {
    int status;

    if (!buf->addr) {
        DSC_ERROR("The buffer points to an invalid address");
        return DSC_EFAULT;
    } else {
        assert((long)(buf->addr) % sysconf(_SC_PAGE_SIZE) == 0);
        status = munmap(buf->addr, buf->bsize);
        buf->addr = NULL;
        buf->bsize = 0;

        if (status != 0) {
            DSC_ERROR("Failed to unmap buffer");
            return DSC_EFREE;
        }
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
DSC_DECL DSC_Error dsc_resize_buffer(restrict pBuffer_t buf, const size_t newlen) {
    if (newlen <= 0) {
        DSC_ERROR("The new buffer length is invalid");
        return DSC_EINVAL;
    } else if (!buf->addr) {
        DSC_ERROR("The buffer points to an invalid address");
        return DSC_EFAULT;
    } else {
        assert((long)(buf->addr) % sysconf(_SC_PAGE_SIZE) == 0);
        buf->addr = mremap(buf->addr, buf->bsize, (newlen * buf->tsize), 0);
        if (MAP_FAILED == buf->addr) {
            DSC_ERROR("Failed to remap buffer");
            buf->addr = NULL;
            return DSC_ERROR;
        }
    }

    buf->bsize = newlen * buf->tsize;
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
DSC_DECL DSC_Error dsc_clear_buffer(buffer_t buf, const uint8_t byte) {
    if (!buf.addr) {
        DSC_ERROR("The buffer points to an invalid address");
        return DSC_EFAULT;
    }

    memset(buf.addr, (int)byte, buf.bsize);
    return DSC_EOK;
}

/**
 * Returns the number of elements that a buffer can fit.
 *
 * @since 04/06/2022
 * @param[in] buf Buffer struct
 * @returns The length of the buffer or DSC_EFAULT upon failure
 */
DSC_DECL size_t dsc_get_buffer_capacity(buffer_t buf) {
    if (!buf.addr) {
        DSC_ERROR("The buffer points to an invalid address");
        return -1;
    } 

    return (buf.bsize / buf.tsize);
}
