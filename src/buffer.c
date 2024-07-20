/**
 * @file buffer.c
 * @author Neil Kingdom
 * @version 1.0
 * @since 04-06-2022
 * @brief Provides APIs for managing a buffer construct.
*/

#include "buffer.h"

/*
 * ===============================
 *       Public Functions
 * ===============================
 */

/**
 * @brief Creates a new general purpose buffer.
 * @since 04/06/2022
 * @param[in] nelem The initial number of elements that the buffer shall contain
 * @param[in] tsize The size (in bytes) of the datatype used for the buffer
 * @returns A new Buffer_t object
 */
DSC_DECL Buffer_t dsc_buf_create(const size_t nelem, const size_t tsize) {
    size_t bsize = nelem * tsize;
    Buffer_t buf = { NULL, tsize, bsize };

    /* Input validation (size_t specifies no upper bound, so we can't check that) */
    if (nelem <= 0 || tsize <= 0) {
        DSC_LOG("Invalid parameter", DSC_ERROR);
        return buf;
    }

    /* Allocate space for the buffer */
    buf.addr = mmap(NULL, bsize, (PROT_READ | PROT_WRITE), (MAP_SHARED | MAP_ANON), -1, 0);
    if (buf.addr == MAP_FAILED) {
        DSC_LOG("Failed to allocate memory for buffer", DSC_ERROR);
        buf.addr = NULL;
    }

    return buf;
}

/**
 * @brief Free an allocated buffer.
 * @since 04/06/2022
 * @param[in/out] buf A pointer to the buffer being destroyed
 * @returns A DscError_t object containing the exit status code
 */
DSC_DECL DscError_t dsc_buf_destroy(Buffer_t * restrict buf) {
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
            DSC_LOG("Failed to unmap buffer", DSC_WARNING);
            return DSC_EFAIL;
        }
    }

    return DSC_EOK;
}

/**
 * @brief Resize an existing buffer.
 * @since 04/06/2022
 * @param[in/out] buf A pointer to the buffer being resized
 * @param[in] nelem The new number of elements that the buffer shall contain
 * @returns A DscError_t object containing the exit status code
 */
DSC_DECL DscError_t dsc_buf_resize(Buffer_t * restrict buf, const size_t nelem) {
    if (nelem <= 0) {
        DSC_LOG("The new buffer length is invalid", DSC_ERROR);
        return DSC_EINVAL;
    } else if (!buf->addr) {
        DSC_LOG("The buffer points to an invalid address", DSC_ERROR);
        return DSC_EFAULT;
    } else {
        buf->addr = mremap(buf->addr, buf->bsize, (nelem * buf->tsize), 0);
        if (buf->addr == MAP_FAILED) {
            DSC_LOG("Failed to remap buffer", DSC_ERROR);
            buf->addr = NULL;
            buf->bsize = 0;
            return DSC_EFAIL;
        }
    }

    buf->bsize = nelem * buf->tsize;
    return DSC_EOK;
}

/**
 * @brief Writes the value contained in "byte" to each byte of the buffer.
 * @since 04/06/2022
 * @param[in/out] buf A pointer to the buffer being filled
 * @param[in] byte The byte to fill the buffer with
 * @returns A DscError_t object containing the exit status code
 */
DSC_DECL DscError_t dsc_buf_fill(Buffer_t * restrict buf, const uint8_t byte) {
    if (!buf->addr) {
        DSC_LOG("The buffer points to an invalid address", DSC_ERROR);
        return DSC_EFAULT;
    }

    memset(buf->addr, (int)byte, buf->bsize);
    return DSC_EOK;
}

/**
 * @brief Returns the number of elements that the buffer can fit.
 * @since 04/06/2022
 * @param[in] buf The buffer whos capacity is being checked
 * @returns The capacity of the buffer or DSC_EFAIL upon failure
 */
DSC_DECL ssize_t dsc_buf_capacity(const Buffer_t buf) {
    if (!buf.addr) {
        DSC_LOG("The buffer points to an invalid address", DSC_ERROR);
        return DSC_EFAIL;
    }

    return (buf.bsize / buf.tsize);
}
