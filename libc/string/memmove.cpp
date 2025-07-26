#include <string.h>

void*
memmove(void* dest, const void* src, size_t n)
{
  const unsigned char* _src = reinterpret_cast<const unsigned char*>(src);
  unsigned char* _dest = reinterpret_cast<unsigned char*>(dest);

  if (_src == _dest || n == 0)
    return _dest;
  if (_dest > _src && _dest - _src < (long)n) {
    size_t i;
    // The code `n-n` is there to simply shut off the warning
    // that says for unsigned types, comparison >= 0 is
    // always true. I just used `n-n` to replace `0`.
    // for (i = n - 1; i >= 0; i--)
    for (i = n - 1; i >= (n - n); i--)
      _dest[i] = _src[i];
    return _dest;
  }
  if (_src > _dest && _src - _dest < (long)n) {
    size_t i;
    for (i = 0; i < n; i++)
      _dest[i] = _src[i];
    return _dest;
  }
  memcpy(_dest, _src, n);
  return dest;
}