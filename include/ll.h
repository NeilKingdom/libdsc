#ifndef LL_H
#define LL_H

#include "dsc_common.h"
#include "buffer.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct LLNode {
   struct LLNode *next; /* Pointer to next node in the list */
   Buffer_t       data; /* Buffer_t struct containing the node's data */
} LLNode_t;

/* Forward function declarations */

DSC_DECL LLNode_t*      dsc_ll_create(const size_t tsize, const void* const data);
DSC_DECL DscError_t     dsc_ll_destroy(LLNode_t *head);
DSC_DECL DscError_t     dsc_ll_fill(LLNode_t *head, const uint8_t byte);
DSC_DECL DscError_t     dsc_ll_add_node(LLNode_t *head, const void* const data);
DSC_DECL DscError_t     dsc_ll_remove_node(LLNode_t *head, const unsigned idx);
/* TODO: Add insert_node */
DSC_DECL LLNode_t*      dsc_ll_retrieve_node(LLNode_t *head, const unsigned idx);
DSC_DECL ssize_t        dsc_ll_num_nodes(LLNode_t *head);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LL_H */
