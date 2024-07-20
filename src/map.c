/**
 * @file map.c
 * @author Neil Kingdom
 * @version 1.0
 * @since 20-07-2024
 * @brief Provies APIs for managing a map construct.
 */

#include "map.h"

/*
 * ===============================
 *       Public Functions
 * ===============================
 */

DSC_DECL Map_t dsc_map_create(
    const size_t nelem,
    const size_t key_tsize,
    const size_t value_tsize
) {
    Map_t map = { 0 };

    map.keys   = dsc_buf_create(nelem, key_tsize);
    map.values = dsc_buf_create(nelem, value_tsize);

    return map;
}

DSC_DECL DscError_t dsc_map_destroy(Map_t map) {
    int status;

    status = dsc_buf_destroy(&map.keys);
    if (status != DSC_EOK) {
        return DSC_EFAIL;
    }

    status = dsc_buf_destroy(&map.values);
    if (status != DSC_EOK) {
        return DSC_EFAIL;
    }

    return DSC_EOK;
}

DSC_DECL DscError_t dsc_add_map_entry(
    Map_t map,
    const void* const key,
    const void* const value
) {
    int status;
    size_t keys_nelem   = dsc_buf_capacity(map.keys);
    size_t values_nelem = dsc_buf_capacity(map.values);

    status = dsc_buf_resize(&map.keys, keys_nelem + 1);
    if (status != DSC_EOK) {
        return DSC_EFAIL;
    }
    status = dsc_buf_resize(&map.values, values_nelem + 1);
    if (status != DSC_EOK) {
        return DSC_EFAIL;
    }

    memcpy((map.keys.addr + keys_nelem), key, map.keys.tsize);
    memcpy((map.values.addr + values_nelem), value, map.values.tsize);

    return DSC_EOK;
}

DSC_DECL DscError_t dsc_map_replace_entry(
    Map_t map,
    const void* const key,
    const void* const value
) {
    size_t i, nelem = dsc_buf_capacity(map.keys);

    for (i = 0; i < nelem; i += map.values.tsize) {
        if (memcmp((map.keys.addr + i), key, map.keys.tsize) == 0) {
            memcpy((map.values.addr + i), value, map.values.tsize);
            return DSC_EOK;
        }
    }

    return DSC_EFAIL;
}

DSC_DECL DscError_t dsc_map_remove_entry(Map_t map, const void* const key) {
    size_t i, nelem = dsc_buf_capacity(map.keys);

    for (i = 0; i < nelem; i += map.values.tsize) {
        if (memcmp((map.keys.addr + i), key, map.keys.tsize) == 0) {
            /* TODO: How to move everything over? Or do we? */
            return DSC_EOK;
        }
    }

    return DSC_EFAIL;
}

DSC_DECL Buffer_t dsc_map_retrieve_value(const Map_t map, const void* const key) {
    size_t i, nelem = dsc_buf_capacity(map.keys);
    Buffer_t value = { 0 };

    for (i = 0; i < nelem; i += map.values.tsize) {
        if (memcmp((map.keys.addr + i), key, map.keys.tsize) == 0) {
            value = dsc_buf_create(1, map.values.tsize);
            memcpy(value.addr, (map.values.addr + i), map.values.tsize);
            break;
        }
    }

    return value;
}

DSC_DECL bool dsc_map_contains_key(const Map_t map, const void* const key) {
    size_t i, nelem = dsc_buf_capacity(map.keys);

    for (i = 0; i < nelem; i += map.keys.tsize) {
        if (memcmp((map.keys.addr + i), key, map.keys.tsize) == 0) {
            return true;
        }
    }

    return false;
}

DSC_DECL bool dsc_map_contains_value(const Map_t map, const void* const value) {
    size_t i, nelem = dsc_buf_capacity(map.values);

    for (i = 0; i < nelem; i += map.values.tsize) {
        if (memcmp((map.values.addr + i), value, map.values.tsize) == 0) {
            return true;
        }
    }

    return false;
}
