#ifndef MAP_H
#define MAP_H

#include "dsc_common.h"
#include "buffer.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct {
    Buffer_t    keys;
    Buffer_t    values;
} *pMap_t, Map_t;

/* Forward function declarations */

DSC_DECL Map_t          dsc_create_map(const size_t nelem, const size_t k_tsize, const size_t v_tsize);
DSC_DECL DscError_t     dsc_clear_map(Map_t map);
DSC_DECL bool           dsc_map_contains_key(const Map_t map, const void *key);
DSC_DECL bool           dsc_map_contains_value(const Map_t map, const void *value);
DSC_DECL DscError_t     dsc_add_map_entry(Map_t map, pBuffer_t key, pBuffer_t value);
DSC_DECL DscError_t     dsc_remove_map_entry(Map_t map, pBuffer_t key, pBuffer_t value);
DSC_DECL DscError_t     dsc_replace_map_entry(Map_t map, pBuffer_t old_key, pBuffer_t new_key);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MAP_H */
