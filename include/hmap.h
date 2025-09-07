#ifndef HMAP_H
#define HMAP_H

#include "dsc_common.h"
#include "map.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// Forward function declarations

DscError_t     dsc_hmap_init(Map_t *map, const size_t nelem);
DscError_t     dsc_hmap_add_entry(Map_t map, const void* const key, const void* const value);
DscError_t     dsc_hmap_replace_entry(Map_t map, const void* const key, const void* const value);
DscError_t     dsc_hmap_remove_entry(Map_t map, const void* const key);
Buffer_t       dsc_hmap_retrieve_value(const Map_t map, const void* const key);
bool           dsc_hmap_contains_key(const Map_t map, const void* const key);
bool           dsc_hmap_contains_value(const Map_t map, const void* const value);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // HMAP_H
