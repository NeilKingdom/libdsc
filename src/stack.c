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

DSC_DECL Stack_t dsc_stack_create(const void* const data, const size_t tsize) {
    Stack_t stack = dsc_buf_create(1, tsize);

    if (data != NULL) {
        memcpy(stack.addr, data, tsize);
    }

    return stack;
}

DSC_DECL DscError_t dsc_stack_destroy(Stack_t *stack) {
    if (stack == NULL) {
        DSC_LOG("The stack points to an invalid address", DSC_ERROR);
        return DSC_EINVAL;
    }

    return dsc_buf_destroy(stack);
}

DSC_DECL DscError_t dsc_stack_push(Stack_t *stack, const void* const data) {
    int status;
    size_t nelem = dsc_stack_capacity(*stack);

    if (stack == NULL) {
        DSC_LOG("The stack points to an invalid address", DSC_ERROR);
        return DSC_EINVAL;
    }

    status = dsc_buf_resize(stack, nelem + 1);
    if (status != DSC_EOK) {
        return DSC_EFAIL;
    }

    if (data != NULL) {
        memcpy((stack->addr + (nelem * stack->tsize)), data, stack->tsize);
    }

    return DSC_EOK;
}

DSC_DECL Buffer_t dsc_stack_pop(Stack_t *stack) {
    Buffer_t buf = { 0 };
    void *data_bak = NULL;
    size_t nelem = dsc_buf_capacity(*stack);

    if (stack == NULL) {
        DSC_LOG("The stack points to an invalid address", DSC_ERROR);
        return buf;
    }

    buf = dsc_buf_create(1, stack->tsize);
    if (buf.addr != NULL) {
        memcpy(buf.addr, stack->addr + ((nelem - 1) * stack->tsize), stack->tsize);
    }

    if (nelem == 1) {
        dsc_stack_destroy(stack);
    } else {
        dsc_buf_resize(stack, nelem - 1);
    }

    return buf;
}

DSC_DECL Buffer_t dsc_stack_peek(Stack_t stack) {
    int status;
    Buffer_t buf;
    size_t nelem = dsc_buf_capacity(stack);

    buf = dsc_buf_create(1, stack.tsize);
    if (buf.addr != NULL) {
        memcpy(buf.addr, stack.addr + ((nelem - 1) * stack.tsize), stack.tsize);
    }

    return buf;
}

DSC_DECL ssize_t dsc_stack_capacity(Stack_t stack) {
    return dsc_buf_capacity(stack);
}
