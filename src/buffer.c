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
 * @brief Initializes a general purpose buffer.
 * @since 30-08-2025
 * @param[in/out] buf The Buffer_t object to be initialized
 * @param[in] nelem The initial number of elements that the buffer shall contain
 * @param[in] tsize The size (in bytes) of the datatype used for the buffer
 * @returns DSC_EFAIL if buffer could not be initialized, otherwise returns DSC_EOK
 */
DscError_t dsc_buf_init(Buffer_t *buf, const size_t nelem, const uint8_t tsize) {
    const size_t bsize = nelem * tsize;
    buf->base = malloc(bsize);
    if (buf->base == NULL) {
        DSC_LOG("Failed to allocate memory for dsc buffer", DSC_ERROR);
        return DSC_EFAIL;
    }
    buf->bsize = bsize;
    buf->tsize = tsize;

    return DSC_EOK;
}

/**
 * @brief Resize an existing buffer.
 * @since 04/06/2022
 * @param[in] buf A pointer to the buffer being resized
 * @param[in] nelem The new number of elements that the buffer shall contain
 * @returns A DscError_t object containing the exit status code
 */
DscError_t dsc_buf_resize(Buffer_t *buf, const size_t nelem) {
    const size_t bsize = nelem * buf->tsize;
    buf->base = realloc(buf->base, bsize);
    if (buf->base == NULL) {
        DSC_LOG("Failed to allocate memory for dsc buffer", DSC_ERROR);
        return DSC_EFAIL;
    }
    buf->bsize = bsize;

    return DSC_EOK;
}

/**
 * @brief Writes the value contained in "byte" to each byte of the buffer.
 * @since 04/06/2022
 * @param[in] buf A pointer to the buffer being filled
 * @param[in] byte The byte to fill the buffer with
 * @returns A DscError_t object containing the exit status code
 */
DscError_t dsc_buf_fill(Buffer_t *buf, const uint8_t byte) {
    if (buf->base == NULL) {
        DSC_LOG("The buffer points to an invalid baseess", DSC_ERROR);
        return DSC_EFAULT;
    }

    memset(buf->base, (int)byte, buf->bsize);
    return DSC_EOK;
}

/**
 * @brief Returns the number of elements that the buffer can fit.
 * @since 04/06/2022
 * @param[in] buf The buffer whos capacity is being checked
 * @returns The capacity of the buffer or DSC_EFAIL upon failure
 */
size_t dsc_buf_nelem(const Buffer_t* const buf) {
    if (buf->base == NULL) {
        DSC_LOG("The buffer points to an invalid baseess", DSC_ERROR);
        return DSC_EFAIL;
    }

    return (buf->bsize / buf->tsize);
}
