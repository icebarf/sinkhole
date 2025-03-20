#include <stdio.h>
#include <stdlib.h>

void __attribute((noreturn))
abort(void)
{
#if defined(__is_libsinkc)
  // TODO: Proper Kernel Panic
  printf("Kernel: panic condition...\n");
  printf("Kernel: exiting execution - hlt...\n");
  asm volatile("hlt");
#else
  // TODO: Proper userspace abort
  printf("abort()\n");
#endif
  while (1) {
  }
  __builtin_unreachable();
}