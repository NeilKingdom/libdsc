#ifndef MAP_H
#define MAP_H

#include "dsc_common.h"
#include "buffer.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct {
    Buffer_t keys;   /* Buffer containing keys */
    Buffer_t values; /* Buffer containing values */
} Map_t;

/* Forward function declarations */

DSC_DECL Map_t          dsc_map_create(const size_t nelem, const size_t key_tsize, const size_t value_tsize);
DSC_DECL DscError_t     dsc_map_destroy(Map_t map);
DSC_DECL DscError_t     dsc_map_add_entry(Map_t map, const void* const key, const void* const value);
DSC_DECL DscError_t     dsc_map_replace_entry(Map_t map, const void* const key, const void* const value);
DSC_DECL DscError_t     dsc_map_remove_entry(Map_t map, const void* const key);
DSC_DECL Buffer_t       dsc_map_retrieve_value(const Map_t map, const void* const key);
DSC_DECL bool           dsc_map_contains_key(const Map_t map, const void* const key);
DSC_DECL bool           dsc_map_contains_value(const Map_t map, const void* const value);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MAP_H */
