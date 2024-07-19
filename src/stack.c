/**
 * This source file provides basic functionality for
 * creating a generic buffer. It is used by all other
 * data structures, and is thus the cornerstone of the
 * library.
 *
 * @file
 * @author Neil Kingdom
 * @version 1.0
 * @since 04/06/2022
*/

#include "stack.h"

/*
 * ===============================
 *       Private Functions
 * ===============================
 */

static void  _dsc_pop_stack(restrict pStack_t stack) {
    size_t new_size = dsc_get_buffer_capacity(stack->arena) - 1;

    if (new_size == 0) {
        dsc_destroy_stack(stack);
    } else {
        dsc_resize_buffer(&stack->arena, new_size);
        stack->offset -= stack->arena.tsize;
    }

    pop_pending = false;
}

/*
 * ===============================
 *       Public Functions
 * ===============================
 */

/**
 * @brief Creates a new general purpose stack.
 */
DSC_DECL Stack_t dsc_create_stack(const size_t tsize) {
    if (tsize <= 0) {
        DSC_ERROR("Invalid argument");
        return (Stack_t){ 0 };
    }
    return (Stack_t){ .arena = dsc_create_buffer(1, tsize), .offset = 0};
}

DSC_DECL DscError_t dsc_destroy_stack(pStack_t restrict stack) {
    if (!stack->arena.addr) {
        DSC_ERROR("Attempted double free");
        return DSC_EFAIL;
    }

    dsc_destroy_buffer(&stack->arena);
    stack->offset = 0;

    return DSC_EOK;
}

DSC_DECL DscError_t dsc_push_stack(pStack_t restrict stack, void *data) {
    size_t new_size;

    if (pop_pending) {
        _dsc_pop_stack(stack);
    }

    if (!stack->arena.addr) {
        DSC_ERROR("The stack points to an invalid address");
        return DSC_EFAULT;
    } else if (!data) {
        DSC_ERROR("Invalid argument");
        return DSC_EINVAL;
    }

    memcpy(stack->arena.addr + stack->offset, data, stack->arena.tsize);
    new_size = dsc_get_buffer_capacity(stack->arena) + (stack->offset / stack->arena.tsize);
    if (new_size > 0) {
        dsc_resize_buffer(&stack->arena, new_size);
    }
    stack->offset += stack->arena.tsize;

    return DSC_EOK;
}

DSC_DECL void *dsc_pop_stack(pStack_t restrict stack) {
    if (pop_pending) {
        _dsc_pop_stack(stack);
    }

    if (!stack->arena.addr) {
        DSC_ERROR("Attempted double free");
        return NULL;
    } else if (stack->offset == 0) {
        DSC_ERROR("The stack is empty");
        return NULL;
    }

    pop_pending = true;
    assert((stack->offset - stack->arena.tsize) >= 0);
    return (stack->arena.addr + stack->offset - stack->arena.tsize);
}

DSC_DECL void *dsc_peek_stack(pStack_t restrict stack) {
    if (pop_pending) {
        _dsc_pop_stack(stack);
    }

    if (!stack->arena.addr) {
        DSC_ERROR("The stack points to an invalid address");
        return NULL;
    } else if (stack->offset == 0) {
        DSC_ERROR("The stack is empty");
        return NULL;
    }

    assert((stack->offset - stack->arena.tsize) >= 0);
    return (stack->arena.addr + stack->offset - stack->arena.tsize);
}

DSC_DECL ssize_t dsc_get_stack_size(pStack_t restrict stack) {
    if (pop_pending) {
        _dsc_pop_stack(stack);
    }

    if (!stack->arena.addr) {
        DSC_ERROR("The stack points to an invalid address");
        return DSC_EFAIL;
    }

    return (stack->offset / stack->arena.tsize);
}
