#ifndef DSC_COMMON_H
#define DSC_COMMON_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif // GNU_SOURCE

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
#endif // __cplusplus

// Errors align with those defined in errno.h
typedef enum {
    DSC_EFAIL       = -1,   // General purpose error
    DSC_EOK         =  0,   // No error
    DSC_ENOMEM      =  12,  // Not enough memory
    DSC_EFAULT      =  14,  // Bad address
    DSC_EINVAL      =  22,  // The argument was invalid
    DSC_ENODATA     =  61,  // No data available
    DSC_EOVERFLOW   =  75,  // Value too large to be stored in data type
} DscError_t;

typedef enum {
    DSC_NOTE,
    DSC_WARNING,
    DSC_ERROR
} DscLogLevel_t;

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#elif defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"
#endif

static void _dsc_log(
    const char *file,
    const char *func,
    const int line,
    const char *msg,
    const DscLogLevel_t level
) {
    const char* header;
    switch (level) {
        case DSC_NOTE:
            header = strdup("NOTE");
            break;
        case DSC_WARNING:
            header = strdup("WARNING");
            break;
        case DSC_ERROR:
            header = strdup("ERROR");
            break;
    }

    fprintf((level == DSC_NOTE) ? stdout : stderr,
        "\n=========== %s ===========\n"
        "Logger: %s\n"
        "File: %s, Function: %s, Line: %d\n",
        header, msg, file, func, line
    );
}

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#elif defined(__clang__)
#pragma clang diagnostic pop
#endif

#define DSC_LOG(msg, level) do { \
    _dsc_log((__FILE__), (__func__), (__LINE__), (msg), (level)); \
} while (0)

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // COMMON_H
