#ifndef HASH_H
#define HASH_H

#include "dsc_common.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * @brief Produces a 32-bit hash for a generic byte stream using FNV-1a hash.
 * @since 06-09-2025
 * @param[in] data The data being hashed
 * @param[in] size The size of __data__ in bytes
 * @returns The 32-bit hash
 */
static uint32_t fnv1a_hash(const void *data, const size_t size) {
    const uint8_t *bytes = (const uint8_t *)data;
    uint32_t hash = 2166136261; // FNV offset basis

    for (size_t i = 0; i < size; ++i) {
        hash ^= bytes[i];
        hash *= 16777619; // Multiply by FNV prime
    }

    return hash;
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // HASH_H
