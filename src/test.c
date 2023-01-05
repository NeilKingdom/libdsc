#include "buffer.h"

#define BUF_TYPE int
#define TSIZE sizeof(BUF_TYPE)
#define BLEN (size_t)10

int main(void)
{
   pBuffer_t buf;
   const char *test_msg = "Hello World!";

   buf = create_buffer(BLEN, TSIZE);
   printf("The length of buffer buf is: %ld\n", buffer_len(buf));

   resize_buffer(20, buf);
   printf("The new length of buffer buf is: %ld\n", buffer_len(buf));

   memcpy(DSC_BUF_VOID(buf), (void *)test_msg, strlen(test_msg) + 1);
   printf("Buffer says: %s\n", DSC_BUF_CHAR(buf));

   free_buffer(buf);
   if (!DSC_BUF_VOID(buf))
      printf("Successfully freed buffer!\n");

   return 0;
}
