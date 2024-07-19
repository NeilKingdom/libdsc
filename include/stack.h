#ifndef STACK_H
#define STACK_H

#include "dsc_common.h"
#include "buffer.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct stack {
    Buffer_t arena;  /* The resizable memory arena for the stack's contents */
    size_t   offset; /* An offset into the memory arena */
} *pStack_t, Stack_t;

static bool pop_pending = false; /* Used so that we can return the data at the address before resizing buffer */

/* Forward function declarations */

DSC_DECL Stack_t        dsc_create_stack(const size_t tsize);
DSC_DECL ssize_t        dsc_get_stack_size(pStack_t stack);
DSC_DECL void          *dsc_peek_stack(pStack_t stack);
DSC_DECL void          *dsc_pop_stack(pStack_t stack);
DSC_DECL DscError_t     dsc_push_stack(pStack_t stack, void *data);
DSC_DECL DscError_t     dsc_destroy_stack(pStack_t stack);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* STACK_H */
