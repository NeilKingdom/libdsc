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

DscError_t dsc_stack_init(Stack_t *stack, void *data, const uint8_t tsize) {
    dsc_buf_init((Buffer_t*)stack, 1, tsize);

    if (stack->base == NULL) {
        DSC_LOG("Failed to allocate memory for stack", DSC_ERROR);
        return DSC_EFAIL;
    }
    stack->base = data;

    return DSC_EOK;
}

DscError_t dsc_stack_push(Stack_t *stack, void *data) {
    if (stack == NULL) {
        DSC_LOG("The stack points to an invalid address", DSC_ERROR);
        return DSC_EINVAL;
    }

    size_t nelem = dsc_buf_nelem((Buffer_t*)stack);
    DscError_t status = dsc_buf_resize((Buffer_t*)stack, nelem + 1);
    if (status != DSC_EOK) {
        return DSC_EFAIL;
    }

    if (data != NULL) {
        memcpy((stack->base + (nelem * stack->tsize)), data, stack->tsize);
    }

    return DSC_EOK;
}

// NOTE: Doesn't return popped value cuz we'd have to malloc it and I don't like that
DscError_t dsc_stack_pop(Stack_t *stack) {
    if (stack == NULL) {
        DSC_LOG("The stack points to an invalid address", DSC_ERROR);
        return DSC_EINVAL;
    }

    size_t nelem = dsc_buf_nelem((Buffer_t*)stack);
    if (nelem == 1) {
        stack->bsize = 0;
        stack->base = NULL;
    } else {
        DscError_t status = dsc_buf_resize((Buffer_t*)stack, nelem - 1);
        if (status != DSC_EOK) {
            return DSC_EFAIL;
        }
    }

    return DSC_EOK;
}

void *dsc_stack_peek(const Stack_t* const stack) {
    size_t nelem = dsc_buf_nelem((Buffer_t*)stack);
    return stack->base + ((nelem - 1) * stack->tsize);
}

size_t dsc_stack_nelem(const Stack_t* const stack) {
    return dsc_buf_nelem((const Buffer_t* const)stack);
}
