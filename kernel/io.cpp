#include <kernel/io/io.hpp>
#include <kernel/io/serial.hpp>
#include <kernel/io/vga_tty.hpp>

void
kputchar(char c)
{
  vga_term_putchar(c);
#ifdef __SINKHOLE_KERNEL_DO_SERIAL_IO_DEBUG
  serial_write(c);
#endif
}