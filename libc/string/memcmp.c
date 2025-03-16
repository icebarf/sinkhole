#include <string.h>

int
memcmp(const void* lhs, const void* rhs, size_t count)
{
  int rvalue = 0;
  rvalue = strncmp(lhs, rhs, count);

  return rvalue;
}