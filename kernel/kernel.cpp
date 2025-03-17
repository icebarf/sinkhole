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
  for (int i = 0; i < 23; i++)
    printf("%d\n", i);
  int v = init_serial();
  if (v >= 1) {
    printf("sinkhole: serial: initialization success with code %d\n", v);
    return;
  }
  printf("sinkhole: serial: %s with code %d\n", "initialization success", v);
}
