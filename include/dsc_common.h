#ifndef DSC_COMMON_H
#define DSC_COMMON_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/mman.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Change to static if you want function declarations to be static */
#define DSC_DECL

typedef enum {
   DSC_EOK = 0, /* No error */
   DSC_ERROR,   /* General error */
   DSC_EINVAL,  /* The argument was invalid */
   DSC_ENOMEM,  /* Not enough memory */
   DSC_EFREE,   /* Error freeing memory */
   DSC_EFAULT   /* Bad address */
} DSC_Error;

#if defined(__GNUC__)
#pragma GCC diagnostic push 
#pragma GCC diagnostic ignored "-Wunused-function"
#elif defined(__clang__)
#pragma clang diagnostic push 
#pragma clang diagnostic ignored "-Wunused-function"
#endif
static void _dsc_error(char *file, const char *func, int line, const char *msg) {
   fprintf(stderr, "\n=========== ERROR ===========\n"
                   "Backtrace:\n\n"
                   "File: %s, Function: %s, Line: %d\n"
                   "Short message: %s\n"
                   "Errno message: %s\n",
                   file, func, line, msg, strerror(errno));
}
#if defined(__GNUC__)
#pragma GCC diagnostic pop
#elif defined(__clang__)
#pragma clang diagnostic pop
#endif

#define DSC_ERROR(msg) (_dsc_error((__FILE__), (__func__), (__LINE__), (msg)))

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* COMMON_H */
