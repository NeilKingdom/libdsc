#define DSC_STATIC static

#define DSC_ERROR(file, func, line) printf("Error: File %s, Function %s, Line %d\n",\
                                    (file), (func), (line))

enum DSC_Error {
                  DSC_ERROR = 1,
                  DSC_ENOMEM,
                  DSC_EDFREE
               };
