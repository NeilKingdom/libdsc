#ifndef STACK_H
#define STACK_H

#include "dsc_common.h"
#include "buffer.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef Buffer_t Stack_t; /* Stack is just a buffer of buffers */

/* Forward function declarations */

DSC_DECL Stack_t        dsc_stack_create(const void* const data, const size_t tsize);
DSC_DECL DscError_t     dsc_stack_destroy(Stack_t *stack);
DSC_DECL DscError_t     dsc_stack_push(Stack_t *stack, const void* const data);
DSC_DECL Buffer_t       dsc_stack_pop(Stack_t *stack);
DSC_DECL Buffer_t       dsc_stack_peek(const Stack_t stack);
DSC_DECL ssize_t        dsc_stack_capacity(const Stack_t stack);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* STACK_H */
