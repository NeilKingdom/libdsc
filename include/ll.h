#ifndef LL_H
#define LL_H

#include "dsc_common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct LLNode {
    void          *data; // Pointer to the node's data
    struct LLNode *next; // Pointer to next node in the list
} LLNode_t;

LLNode_t       *dsc_ll_create(void *data);
DscError_t      dsc_ll_destroy(LLNode_t *head);
DscError_t      dsc_ll_append(LLNode_t **head, void *data);
DscError_t      dsc_ll_insert(LLNode_t **head, LLNode_t *node, const u32 idx);
DscError_t      dsc_ll_remove(LLNode_t **head, const u32 idx);
const LLNode_t *dsc_ll_peek(const LLNode_t *head, const u32 idx);
usize           dsc_ll_nelem(const LLNode_t *head);

#ifdef __cplusplus
}
#endif

#endif
