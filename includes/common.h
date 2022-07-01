#ifndef COMMON_H
#define COMMON_H

/* Change to static if you want function declarations to be static */
#define DSC_DECL

#define DSC_ERROR(file, func, line) printf("Error: File %s, Function %s, Line %d\n",\
                                    (file), (func), (line))

enum DSC_Types {
                  DSC_BUFFER = 0,
                  DSC_BINTREE,
                  DSC_HASHMAP,
                  DSC_LL,
                  DSC_DLL,
                  DSC_QUEUE,
                  DSC_DEQUEUE,
                  DSC_STACK,
                  DSC_CACHE
               };
enum DSC_Error {
                  DSC_EOK = 0, /* No error */
                  DSC_ERROR,   /* General error */
                  DSC_ENOMEM,  /* Not enough memory */
                  DSC_EFREE    /* Error freeing memory */
               };

/*
 Make sure each member of the node union is same size (sizeof(pointer) * 3)
 Struct declarations are defined internally so that they cannot be created elsewhere
 */
typedef union node
{
   /* Node with data */
   struct nd
   {
      void *data;
      void *padding[2];
   };
   /* Node with data and next ref. */
   struct ndn
   {
      void *data;
      ndn  *next;
      void *padding;
   };
   /* Node with data, next ref., and tail ref. */
   struct ndnp
   {
      void *data;
      ndnp *next;
      ndnp *prev;
   };
} *pNode_t, node_t,
  *pHead_t, head_t,
  *pTail_t, tail_t;

#endif
