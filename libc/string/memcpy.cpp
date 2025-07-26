#include <string.h>

void*
memcpy(void* __restrict dest, const void* __restrict src, size_t n)
{
  unsigned char* _dest = reinterpret_cast<unsigned char*>(dest);
  const unsigned char* _src = reinterpret_cast<const unsigned char*>(src);

  while (n--) {
    *_dest++ = *_src++;
  }
  return dest;
}
