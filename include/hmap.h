#ifndef HMAP_H
#define HMAP_H

#include "dsc_common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DEF_CAPACITY KB(1)

typedef struct {
    u32   k;
    void *v;
    usize v_sz;
} KV_t;

typedef struct {
    KV_t *base;     // Pointer to the base address of the map
    usize capacity; // Number of slots allocated; not the number of KV pairs
} HMap_t;

/*
 * ===============================
 *       Private Functions
 * ===============================
 */

/**
 * @brief Produces a 32-bit hash for a generic byte stream using FNV-1a hash.
 * @since 06-09-2025
 * @param[in] data The data being hashed
 * @param[in] size The size of __data__ in bytes
 * @returns The 32-bit hash
 */
static inline u32 _fnv1a_hash(const void *data, const usize size) {
    const u8 *bytes = (const u8*)data;
    u32 hash = 2166136261; // FNV offset basis

    for (usize i = 0; i < size; ++i) {
        hash ^= bytes[i];
        hash *= 16777619; // Multiply by FNV prime
    }

    return hash;
}

/*
 * ===============================
 *       Public Functions
 * ===============================
 */

static inline HMap_t *dsc_hmap_create(void) {
    HMap_t *hmap = malloc(sizeof(HMap_t));
    if (hmap == NULL) {
        DSC_LOG("Failed to allocate memory for hashmap", DSC_ERROR);
        return NULL;
    }

    const usize capacity = DEF_CAPACITY;
    const usize total_sz = capacity * sizeof(KV_t);

    hmap->base = malloc(total_sz);
    if (hmap->base == NULL) {
        DSC_LOG("Failed to allocate memory for hashmap", DSC_ERROR);
        free(hmap);
        return NULL;
    }
    memset(hmap->base, 0, total_sz);
    hmap->capacity = capacity;

    return hmap;
}

static inline DscError_t dsc_hmap_destroy(HMap_t *map) {
    if (map == NULL) {
        DSC_LOG("Map points to an invalid address", DSC_ERROR);
        return DSC_INVAL;
    }

    if (map->base != NULL) {
        free(map->base);
    }
    free(map);

    return DSC_SUCCESS;
}

// TODO: Dynamic regrowth after exceeding load factor?
static inline DscError_t dsc_hmap_add_entry(HMap_t *map, void *data, const usize size) {
    if (map == NULL) {
        DSC_LOG("Map points to an invalid address", DSC_ERROR);
        return DSC_INVAL;
    }

    KV_t pair = { .v = data, .v_sz = size };
    u32 idx = pair.k = _fnv1a_hash(pair.v, pair.v_sz) % MIN(map->capacity, UINT32_MAX);

    u32 num_collisions = 0;
    u32 max_collisions = 10;

    // Loop while colliding...
    KV_t *cmp;
    while (true) {
        cmp = map->base + idx;
        if (cmp->v == NULL) {
            break;
        }
        if (num_collisions++ == max_collisions - 1) {
            DSC_LOG("Exceeded maximum number of insertion attempts", DSC_ERROR);
            return DSC_FAILURE;
        }
        idx = (idx + 1) % MIN(map->capacity, UINT32_MAX);
    }

    map->base[idx] = pair;
    return DSC_SUCCESS;
}

// TODO: Delete. Only for debug.
static inline void print(HMap_t *map) {
    for (usize i = 0; i < map->capacity; ++i) {
        if (map->base[i].v == NULL) {
            continue;
        }
        KV_t *pair = map->base + i;
        printf("Idx: %zu -> [%u] : <%*s>\n", i, pair->k, pair->v_sz, pair->v);
    }
}

// TODO:
//static inline DscError_t dsc_hmap_remove_entry(Map_t *map, const KV_t kv);
//static inline DscError_t dsc_hmap_replace_entry(Map_t *map, const KV_t kv);
//static inline void *dsc_hmap_get_value(const Map_t *map, void *k);
//static inline void *dsc_hmap_get_key(const Map_t *map, void *v);
//static inline bool dsc_hmap_has_key(const Map_t *map, void *k);
//static inline bool dsc_hmap_has_value(const Map_t *map, void *v);

#ifdef __cplusplus
}
#endif

#endif
