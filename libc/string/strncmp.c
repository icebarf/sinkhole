#include <string.h>

int
strncmp(const char* lhs, const char* rhs, size_t n)
{
  int rvalue = 0; /* return value */

  /* Return zero if count is zero */
  if (n == 0) {
    return 0;
  }

  /* compare individual character values, if they're unequal
   * subtract rhs from lhs to get the lexicographical value */
  while (n--) {
    if (*lhs++ == *rhs++) {
      continue;
    } else {
      rvalue = *(--lhs) - *(--rhs);
    }
  }

  return rvalue;
}