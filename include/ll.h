/**
 * Example Usage:
 *
 * #include <string.h>
 * 
 * #include "./include/ll.h"
 * 
 * #define BUF_TYPE char
 * #define TSIZE sizeof(BUF_TYPE)
 * 
 * int main(void) {
 *    size_t len, i;
 *    pNode_t head = NULL;
 *    const char *word1 = "Hello";
 *    const char *word2 = ", ";
 *    const char *word3 = "World"; 
 * 
 *    head =  create_ll(strlen(word1) + 1, TSIZE, (void *)word1);
 *    add_ll_node(head, strlen(word2) + 1, TSIZE, (void *)word2);
 *    add_ll_node(head, strlen(word3) + 1, TSIZE, (void *)word3);
 * 
 *    len = get_ll_len(head);
 *    printf("Length of linked list: %ld\n", len);
 * 
 *    for (i = 0; i < len; ++i) {
 *       pNode_t curr_node = get_ll_node(head, i);
 *       printf("%s", DSC_BUF_CHAR(curr_node->data));
 *    }
 * 
 *    putchar('\n');
 *    delete_ll(head);
 * }
 */

#ifndef LL_H
#define LL_H

#include "common.h"
#include "buffer.h"

typedef struct Node {
   struct Node *next; /* Pointer to next node in the list */
   pBuffer_t data;    /* Pointer to a buffer_t struct containing the data */
} *pNode_t, node_t;

/* Forward function declarations */
DSC_DECL pNode_t     create_ll(size_t len, size_t tsize, void *data);
DSC_DECL int         add_ll_node(pNode_t head, size_t len, size_t tsize, void *data);
DSC_DECL int         free_ll_node(pNode_t head, unsigned index);
DSC_DECL ssize_t     get_ll_len(pNode_t head);
DSC_DECL pNode_t     get_ll_node(pNode_t head, unsigned index);
DSC_DECL int         clear_ll_data(pNode_t head, uint8_t byte);
DSC_DECL int         destroy_ll(pNode_t head);

#endif /* LL_H */
