#include "arena.h"

/**
 * @brief Creates a memory arena of n bytes specified by __capacity__.
 * @since 06-04-2026
 * @param capacity The maximum size of the arena in bytes.
 * @returns Pointer to the newly allocated arena, or NULL if it could not be allocated.
 */
Arena_t *dsc_arena_create(const usize capacity) {
    usize total_sz = ARENA_BASE_POS + capacity;

    Arena_t *arena = malloc(total_sz);
    if (arena == NULL) {
        DSC_LOG("Failed to allocate memory for the arena", DSC_ERROR);
        return NULL;
    }

    arena->capacity = total_sz;
    arena->next = ARENA_BASE_POS;
    arena->mark = ARENA_BASE_POS;

    return arena;
}

/**
 * @brief Destroys the arena pointed to by __arena__.
 * @since 06-04-2026
 * @param arena Reference to the arena struct to be destroyed.
 * @returns An error code corresponding to the result of the operation.
 */
DscError_t dsc_arena_destroy(Arena_t *arena) {
    if (arena == NULL) {
        DSC_LOG("The arena points to an invalid address", DSC_ERROR);
        return DSC_INVAL;
    }

    free(arena);
    return DSC_SUCCESS;
}

/**
 * @brief Reserve __size__ bytes in the arena.
 * @note Reserved memory will be zeroized.
 * @since 06-04-2026
 * @param arena Reference to the arena object.
 * @param size The size in bytes to reserve within the memory arena.
 * @returns Pointer to the beginning of the reserved space or NULL if the operation failed.
 */
void *dsc_arena_reserve(Arena_t *arena, const usize size) {
    if (arena == NULL) {
        DSC_LOG("The arena points to an invalid address", DSC_ERROR);
        return NULL;
    }

    u64 off_aligned = ROUND_TO_NEAREST_POW2(arena->next, ARENA_ALIGN);
    u64 new_next = off_aligned + size;
    if (size > (arena->capacity - off_aligned)) {
        DSC_LOG("Arena capacity is not large enough to reserve the number of bytes requested", DSC_ERROR);
        return NULL;
    }

    void *mem = (u8*)arena + off_aligned;
    memset(mem, 0, size);
    arena->next = new_next;

    return mem;
}

/**
 * @brief Marks the current location of the arena to be popped to later.
 * @since 07-04-2026
 * @param arena Reference to the arena object.
 * @returns A DscError_t indicating the result of the operation.
 */
DscError_t dsc_arena_mark(Arena_t *arena) {
    if (arena == NULL) {
        DSC_LOG("The arena points to an invalid address", DSC_ERROR);
        return DSC_INVAL;
    } else if (arena->next > arena->capacity || arena->next < ARENA_BASE_POS) {
        DSC_LOG("Tried marking an invalid address", DSC_ERROR);
        return DSC_FAILURE;
    }

    arena->mark = arena->next;
    return DSC_SUCCESS;
}

/**
 * @brief Pops to a previously marked location with dsc_arena_mark().
 * @since 07-04-2026
 * @param arena Reference to the arena object.
 * @returns A DscError_t indicating the result of the operation.
 */
DscError_t dsc_arena_restore(Arena_t *arena) {
    if (arena == NULL) {
        DSC_LOG("The arena points to an invalid address", DSC_ERROR);
        return DSC_INVAL;
    } else if (arena->mark > arena->next || arena->mark < ARENA_BASE_POS) {
        DSC_LOG("Tried popping to an invalid address", DSC_ERROR);
        return DSC_FAILURE;
    }

    arena->next = arena->mark;
    return DSC_SUCCESS;
}

/**
 * @brief Resets/clears the arena without actually destroying it.
 * @since 07-04-2026
 * @param arena Reference to the arena to be reset.
 * @returns A DscError_t indicating the result of the operation.
 */
DscError_t dsc_arena_reset(Arena_t *arena) {
    if (arena == NULL) {
        DSC_LOG("The arena points to an invalid address", DSC_ERROR);
        return DSC_INVAL;
    }

    arena->next = ARENA_BASE_POS;
    arena->mark = ARENA_BASE_POS;

    return DSC_SUCCESS;
}
