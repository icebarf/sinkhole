#include <stdio.h>

#include <kernel/serial>
#include <kernel/vga_tty>

extern "C" void
kernel_main()
{
  vga_term_init();
  printf("Hello, kernel world!\n");
  printf("no - %d\n", 25);
  int v = init_serial();
  if (v >= 1) {
    printf("sinkhole: serial: %s with code %d\n", "initialization failed", v);
    return;
  }
  printf("sinkhole: serial: %s with code %d\n", "initialization success", v);
}
