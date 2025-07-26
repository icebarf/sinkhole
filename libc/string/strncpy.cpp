#include <string.h>

char*
strncpy(char* __restrict dest, const char* __restrict src, size_t n)
{
  size_t count = 0;  /* length of string */
  size_t rbytes = 0; /* remaining bytes */

  /* if "n" bytes to be copied is larger than length of src then
   * write additional zero bytes to dest upto n */
  if (n > (count = strlen(src))) {
    while (*src != '\0') {
      *dest++ = *src++;
    }

    rbytes = n - count;
    while (rbytes--) {
      *dest++ = '\0';
    }

    return dest;
  }

  /* normal copying */
  while (n--) {
    *dest++ = *src++;
  }

  return dest;
}
