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

DSC_DECL Stack_t dsc_stack_create(const size_t tsize) {
    return dsc_buf_create(1, tsize);
}

DSC_DECL DscError_t dsc_stack_destroy(Stack_t *stack) {
    return dsc_buf_destroy(stack);
}

DSC_DECL DscError_t dsc_stack_push(Stack_t *stack, const void* const data) {
    int status;
    size_t nelem = dsc_buf_capacity(*stack);

    status = dsc_buf_resize(stack, nelem + 1);
    if (status != DSC_EOK) {
        return DSC_EFAIL;
    }

    if (data != NULL) {
        memcpy((stack->addr + nelem), data, stack->tsize);
    }

    return DSC_EOK;
}

DSC_DECL Buffer_t dsc_stack_pop(Stack_t *stack) {
    Buffer_t buf;
    size_t nelem = dsc_buf_capacity(*stack);

    buf = dsc_buf_create(1, stack->tsize);
    if (buf.addr != NULL) {
        memcpy(buf.addr, (stack->addr + (nelem - 1)), stack->tsize);
    }

    dsc_buf_resize(stack, nelem - 1);
    return buf;
}

DSC_DECL Buffer_t dsc_stack_peek(Stack_t stack) {
    int status;
    Buffer_t buf;
    size_t nelem = dsc_buf_capacity(stack);

    buf = dsc_buf_create(1, stack.tsize);
    if (buf.addr != NULL) {
        memcpy(buf.addr, (stack.addr + (nelem - 1)), stack.tsize);
    }

    return buf;
}

DSC_DECL ssize_t dsc_stack_capacity(Stack_t stack) {
    return dsc_buf_capacity(stack);
}
