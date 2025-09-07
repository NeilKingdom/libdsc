#ifndef STACK_H
#define STACK_H

#include "dsc_common.h"
#include "buffer.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef Buffer_t Stack_t; // Stack is basically just a dynamic array

// Forward function declarations

DscError_t     dsc_stack_init(Stack_t *stack, void *data, const uint8_t tsize);
DscError_t     dsc_stack_push(Stack_t *stack, void *data);
DscError_t     dsc_stack_pop(Stack_t *stack);
void*          dsc_stack_peek(const Stack_t* const stack);
size_t         dsc_stack_nelem(const Stack_t* const stack);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // STACK_H
