#include <stdio.h>

#if defined(__is_libsinkc)
#include <kernel/io/io.hpp>
#endif

int
putchar(int ic)
{
#if defined(__is_libsinkc)
  char c = (char)ic;
  kputchar(c);
#else
  // TODO: syscall needs implementation
#endif
  return ic;
}