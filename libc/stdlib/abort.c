#include <stdio.h>
#include <stdlib.h>

void __attribute((noreturn))
abort(void)
{
#if defined(__is_libsinkc)
  // TODO: Proper Kernel Panic
  printf("sinkhole kernel panic, killing myself!\n");
  printf("sinkhole kernel: abort()\n");
  asm volatile("hlt");
#else
  // TODO: Proper userspace abort
  printf("abort()\n");
#endif
  while (1) {
  }
  __builtin_unreachable();
}