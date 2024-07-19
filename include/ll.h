#ifndef LL_H
#define LL_H

#include "dsc_common.h"
#include "buffer.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct LLNode {
   struct LLNode *next; /* Pointer to next node in the list */
   Buffer_t       data; /* Pointer to a Buffer_t struct containing the data */
} *pLLNode_t, LLNode_t;

/* Forward function declarations */

DSC_DECL pLLNode_t      dsc_create_ll(const size_t tsize, void *data);
DSC_DECL DscError_t     dsc_destroy_ll(pLLNode_t head);
DSC_DECL DscError_t     dsc_clear_ll(pLLNode_t head, const uint8_t byte);
DSC_DECL DscError_t     dsc_add_ll_node(pLLNode_t head, void *data);
DSC_DECL DscError_t     dsc_del_ll_node(pLLNode_t head, const unsigned idx);
DSC_DECL pLLNode_t      dsc_get_ll_node(pLLNode_t head, const unsigned idx);
DSC_DECL ssize_t        dsc_get_ll_len(pLLNode_t head);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LL_H */
