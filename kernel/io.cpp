#include <kernel/io>
#include <kernel/serial>
#include <kernel/vga_tty>

void
kputchar(char c)
{
  vga_term_putchar(c);
#ifdef __SINKHOLE_KERNEL_DO_SERIAL_IO_DEBUG
  serial_write(c);
#endif
}