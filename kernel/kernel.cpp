#include <kernel/io>
#include <kernel/serial>
#include <kernel/vga_tty>

#include <stdio.h>

extern "C" void
kernel_main()
{
  // initialize VGA and Serial components
  vga_term_init();
  int v = serial_init();
  printf("sinkhole: vga: initialized\n");
  if (v >= 1) {
    printf("sinkhole: serial: %s with code %d\n", "initialization failure", v);
    return;
  }
  printf("sinkhole: serial: %s with code %d\n", "initialization success", v);

  printf("Hello, kernel world!\n");

  printf("hlt...");
}
