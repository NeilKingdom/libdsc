#include "map.h"
#include "buffer.h"

/**
 * @brief Creates a new map.
 * @returns The initialized Map_t object
 */
DSC_DECL Map_t dsc_create_map(const size_t nelem, const size_t k_tsize, const size_t v_tsize) {
    Map_t map = { NULL, NULL };

    if (nelem <= 0 || k_tsize <= 0 || v_tsize <= 0) {
        DSC_ERROR("Invalid parameter");
        return map;
    }

    map.keys   = dsc_create_buffer(nelem, k_tsize);
    map.values = dsc_create_buffer(nelem, v_tsize);

    return map;
}

/**
 * @brief [TODO:description]
 *
 * @return [TODO:return]
 */
DSC_DECL DscError_t dsc_clear_map(Map_t map) {
    int status;

    status = dsc_destroy_buffer(&map.keys);
    if (status != DSC_EOK) {
        return DSC_EFAIL;
    }
    status = dsc_destroy_buffer(&map.values);
    if (status != DSC_EOK) {
        return DSC_EFAIL;
    }

    return DSC_EOK;
}

/**
 * @brief [TODO:description]
 *
 * @param map [TODO:parameter]
 * @param key [TODO:parameter]
 * @return [TODO:return]
 */
DSC_DECL bool dsc_map_contains_key(const Map_t map, const void *key) {
    size_t i, nelem = dsc_get_buffer_capacity(map.keys);

    for (i = 0; i < nelem; i += map.keys.tsize) {
        if (memcmp((void*)(map.keys.addr + i), key, map.keys.tsize) == 0) {
            return true;
        }
    }

    return false;
}

/**
 * @brief [TODO:description]
 *
 * @param map [TODO:parameter]
 * @param value [TODO:parameter]
 * @return [TODO:return]
 */
DSC_DECL bool dsc_map_contains_value(const Map_t map, const void *value) {
    size_t i, nelem = dsc_get_buffer_capacity(map.values);

    for (i = 0; i < nelem; i += map.values.tsize) {
        if (memcmp((void*)(map.values.addr + i), value, map.values.tsize) == 0) {
            return true;
        }
    }

    return false;
}

/**
 * @brief [TODO:description]
 *
 * @param map [TODO:parameter]
 * @param key [TODO:parameter]
 * @param value [TODO:parameter]
 * @return [TODO:return]
 */
DSC_DECL DscError_t dsc_add_map_entry(Map_t map, pBuffer_t key, pBuffer_t value) {

}

/**
 * @brief [TODO:description]
 *
 * @param map [TODO:parameter]
 * @param key [TODO:parameter]
 * @param value [TODO:parameter]
 * @return [TODO:return]
 */
DSC_DECL DscError_t dsc_remove_map_entry(Map_t map, pBuffer_t key, pBuffer_t value) {

}

/**
 * @brief [TODO:description]
 *
 * @param map [TODO:parameter]
 * @param old_key [TODO:parameter]
 * @param new_key [TODO:parameter]
 * @return [TODO:return]
 */
DSC_DECL DscError_t dsc_replace_map_entry(Map_t map, pBuffer_t old_key, pBuffer_t new_key) {

}
