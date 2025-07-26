#include <string.h>

int
memcmp(const void* lhs, const void* rhs, size_t count)
{
  int rvalue = 0;
  rvalue = strncmp(reinterpret_cast<const char*>(lhs),
                   reinterpret_cast<const char*>(rhs),
                   count);

  return rvalue;
}