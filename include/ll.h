#ifndef LL_H
#define LL_H

#include "common.h"
#include "buffer.h"

typedef struct Node {
   struct Node *next; /* Pointer to next node in the list */
   buffer_t     data; /* Pointer to a buffer_t struct containing the data */
} *pNode_t, node_t;

/* Forward function declarations */

DSC_DECL pNode_t     dsc_create_ll(const size_t len, const size_t tsize, void *data);
DSC_DECL DSC_Error   dsc_destroy_ll(pNode_t head);
DSC_DECL DSC_Error   dsc_clear_ll(pNode_t head, const uint8_t byte);
DSC_DECL DSC_Error   dsc_add_ll_node(pNode_t head, const size_t len, const size_t tsize, void *data);
DSC_DECL DSC_Error   dsc_del_ll_node(pNode_t head, const unsigned index);
DSC_DECL pNode_t     dsc_get_ll_node(pNode_t head, const unsigned index);
DSC_DECL ssize_t     dsc_get_ll_len(pNode_t head);

#endif /* LL_H */
