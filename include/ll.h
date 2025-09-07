#ifndef LL_H
#define LL_H

#include "dsc_common.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct LLNode {
    void *data;          // Pointer to the node's data
    struct LLNode *next; // Pointer to next node in the list
} *LLNode_t;

// Forward function declarations

LLNode_t       dsc_ll_create(void* data);
DscError_t     dsc_ll_destroy(LLNode_t head);
DscError_t     dsc_ll_append(LLNode_t head, void* data);
DscError_t     dsc_ll_insert(LLNode_t head, const LLNode_t node, const unsigned idx);
DscError_t     dsc_ll_remove(LLNode_t head, const unsigned idx);
LLNode_t       dsc_ll_peek(const LLNode_t head, const unsigned idx);
size_t         dsc_ll_nelem(const LLNode_t head);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // LL_H
