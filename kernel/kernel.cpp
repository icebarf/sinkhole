#include <stdio.h>

#include <kernel/vga_tty>

extern "C" void
kernel_main()
{
  vga_term_init();
  printf("Hello, kernel world!\n");
  for (int i = 0; i < 23; i++) {
    puts("nah");
  }
  puts("25");
  puts("26");
  printf("Hello, kernel world!\n");
}
