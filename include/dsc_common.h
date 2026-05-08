#ifndef DSC_COMMON_H
#define DSC_COMMON_H

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
#endif

typedef uint8_t  u8;
typedef int8_t   i8;
typedef uint16_t u16;
typedef int16_t  i16;
typedef uint32_t u32;
typedef int32_t  i32;
typedef uint64_t u64;
typedef int64_t  i64;
typedef float    f32;
typedef double   f64;
typedef size_t   usize;
typedef ssize_t  isize;

// Custom error types
typedef enum {
    DSC_FAILURE    = -1,            // General purpose error
    DSC_SUCCESS    =  0,            // No error
    DSC_NOMEM      =  ENOMEM,       // Not enough memory
    DSC_FAULT      =  EFAULT,       // Bad address
    DSC_INVAL      =  EINVAL,       // The argument was invalid
    DSC_NODATA     =  ENODATA,      // No data available
    DSC_OVERFLOW   =  EOVERFLOW     // Value too large to be stored in data type
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
    const i32 line,
    const char *msg,
    const DscLogLevel_t level
) {
#ifndef ENABLE_LOGS
    return;
#endif

    char *header = NULL;
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

    if (header) {
        free(header);
    }
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
#endif

#endif
