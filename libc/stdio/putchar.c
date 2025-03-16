#include <stdio.h>

#if defined(__is_libk)
#include <kernel/vga_tty>
#endif

int
putchar(int ic)
{
#if defined(__is_libk)
  char c = (char)ic;
  vga_term_putchar(c);
#else
  // TODO: syscall needs implementation
#endif
  return ic;
}