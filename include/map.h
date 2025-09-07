#ifndef MAP_H
#define MAP_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stddef.h>

// Method used for when hash collisions occur
typedef enum {
    BUCKETS,    // Use buckets i.e., value points to a linked list
    INCREMENTAL // Continue iterating until a free slot is available
} MapMethod_t;

typedef struct {
    void *key;
    void *value;
} KV_t;

typedef struct {
    KV_t  *base;                // Pointer to the base address of the map
    size_t nelem;               // Number of slots allocated; not the number of KV pairs
    const MapMethod_t method;   // Mapping method (use buckets or increment when collision occurs)
} Map_t;

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // MAP_H
