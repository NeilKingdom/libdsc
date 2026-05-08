/**
 * @file stack.c
 * @author Neil Kingdom
 * @version 1.0
 * @since 04-06-2022
 * @brief
*/

#include "stack.h"

/*
 * ===============================
 *       Public Functions
 * ===============================
 */

Stack_t dsc_stack_init(const usize max_nelem, const usize type_sz) {
    Stack_t stack = { 0 };

    stack.arena = dsc_arena_create(max_nelem * type_sz);
    if (stack.arena == NULL) {
        DSC_LOG("Failed to allocate memory for stack", DSC_ERROR);
        return stack;
    }
    stack.type_sz = type_sz;

    return stack;
}

DscError_t dsc_stack_destroy(Stack_t *stack) {
    if (stack == NULL) {
        DSC_LOG("The stack points to an invalid address", DSC_ERROR);
        return DSC_EINVAL;
    }

    dsc_arena_destroy(stack->arena);
    stack->arena = NULL;
    stack->type_sz = 0;
    stack->nelem = 0;

    return DSC_EOK;
}

DscError_t dsc_stack_push(Stack_t *stack, void *data) {
    if (stack == NULL) {
        DSC_LOG("The stack points to an invalid address", DSC_ERROR);
        return DSC_EINVAL;
    }

    dsc_arena_mark(stack->arena);
    void *mem = dsc_arena_reserve(stack->arena, stack->type_sz);
    if (mem == NULL) {
        DSC_LOG("Stack failed to allocate enough space for new element", DSC_ERROR);
    }
    memcpy(mem, data, stack->type_sz);
    stack->nelem++;

    return DSC_EOK;
}

DscError_t dsc_stack_pop(Stack_t *stack) {
    if (stack == NULL) {
        DSC_LOG("The stack points to an invalid address", DSC_ERROR);
        return DSC_EINVAL;
    }

    stack->nelem--;

    return DSC_EOK;
}

void *dsc_stack_peek(const Stack_t* const stack) {
}

usize dsc_stack_nelem(const Stack_t* const stack) {
    return stack->nelem;
}
