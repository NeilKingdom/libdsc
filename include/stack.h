#ifndef STACK_H
#define STACK_H

#include "common.h"
#include "buffer.h"

typedef struct stack {
   off_t      stk_off;
   pBuffer_t *base_ptr;
} *pStack_t, stack_t;

DSC_DECL pStack_t create_stack(void);
DSC_DECL int push(pStack_t stack, size_t len, size_t tsize, void *data);
DSC_DECL pBuffer_t pop(pStack_t stack);
DSC_DECL pBuffer_t peek(pStack_t stack);

#endif
