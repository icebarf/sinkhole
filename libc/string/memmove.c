#include <string.h>

void*
memmove(void* dest, const void* src, size_t n)
{
  const unsigned char* _src = src;
  unsigned char* _dest = dest;

  if (_src == _dest || n == 0)
    return _dest;
  if (_dest > _src && _dest - _src < (int)n) {
    size_t i;
    for (i = n - 1; i >= 0; i--)
      _dest[i] = _src[i];
    return _dest;
  }
  if (_src > _dest && _src - _dest < (int)n) {
    size_t i;
    for (i = 0; i < n; i++)
      _dest[i] = _src[i];
    return _dest;
  }
  memcpy(_dest, _src, n);
  return dest;
}