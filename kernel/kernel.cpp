#include <stdio.h>

#include <kernel/serial>
#include <kernel/vga_tty>

// TODO: Issue with printing exactly the 25th line
// output is truncated
extern "C" void
kernel_main()
{
  vga_term_init();
  printf("Hello, kernel world!\n");
  int v = init_serial();
  if (v >= 1) {
    printf("sinkhole: serial: %s with code %d\n", "initialization failure", v);
    return;
  }
  printf("sinkhole: serial: %s with code %d\n", "initialization success", v);
  printf("kernel returning to _start now... hlt!");
}
