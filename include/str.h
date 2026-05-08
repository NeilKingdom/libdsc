#ifndef STR_H
#define STR_H

#include "dsc_common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    void  *base; // Pointer to start of string slice
    size_t len;  // Number of characters
    size_t c_sz; // Character width
} Str_t;

#ifdef __cplusplus
}
#endif

#endif
