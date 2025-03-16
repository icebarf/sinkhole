#include <string.h>

void*
memcpy(void* __restrict dest, const void* __restrict src, size_t n)
{
  unsigned char* _dest = dest;
  const unsigned char* _src = src;

  while (n--) {
    *_dest++ = *_src++;
  }
  return dest;
}
