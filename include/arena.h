#ifndef ARENA_H
#define ARENA_H

#include "dsc_common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ARENA_BASE_POS (sizeof(Arena_t))
#define ARENA_ALIGN (sizeof(*void))
#define ROUND_TO_NEAREST_POW2(n, p) (((u64)(n) + ((u64)(p) - 1)) & (~((u64)p - 1)))

typedef struct {
    usize capacity; // Size of the arena
    u64   next;     // Next reservable address
    u64   mark;     // Bookmarked position to pop to with restore
} Arena_t;

Arena_t   *dsc_arena_create(const usize capacity);
DscError_t dsc_arena_destroy(Arena_t *arena);
void      *dsc_arena_reserve(Arena_t *arena, const usize size);
DscError_t dsc_arena_mark(Arena_t *arena);
DscError_t dsc_arena_restore(Arena_t *arena);
DscError_t dsc_arena_reset(Arena_t *arena);

#ifdef __cplusplus
}
#endif

#endif
