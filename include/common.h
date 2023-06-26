#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <errno.h>
#include <string.h>

/* Change to static if you want function declarations to be static */
#define DSC_DECL

enum DSC_Error {
   DSC_EOK = 0, /* No error */
   DSC_ERROR,   /* General error */
   DSC_EINVAL,  /* The argument was invalid */
   DSC_ENOMEM,  /* Not enough memory */
   DSC_EFREE,   /* Error freeing memory */
   DSC_EFAULT   /* Bad address */
};

static void _dsc_error(char *file, const char *func, int line, const char *msg) {
   fprintf(stderr, "\n=========== ERROR ===========\n" 
                   "Backtrace:\n\n"
                   "File: %s, Function: %s, Line: %d\n"
                   "Short message: %s\n"
                   "Errno message: %s\n\n",
                   file, func, line, msg, strerror(errno));
}

#define DSC_ERROR(msg) (_dsc_error((__FILE__), (__func__), (__LINE__), (msg)))

#endif /* COMMON_H */
