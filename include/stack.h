#ifndef STACK_H
#define STACK_H

#include "dsc_common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    usize capacity;
} Stack_t;

Stack_t    *dsc_stack_create(const usize capacity);
DscError_t  dsc_stack_destroy(Stack_t *stack);
DscError_t  dsc_stack_push(Stack_t *stack, void *data);
DscError_t  dsc_stack_pop(Stack_t *stack);
const void *dsc_stack_peek(const Stack_t* const stack);
usize       dsc_stack_nelem(const Stack_t* const stack);

#ifdef __cplusplus
}
#endif

#endif
