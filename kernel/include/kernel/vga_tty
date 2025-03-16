#ifndef _SINKHOLE_KERNEL_VGA_TTY_H
#define _SINKHOLE_KERNEL_VGA_TTY_H

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

  void vga_term_init();
  void vga_term_putchar(const char c);
  void vga_term_write(const char* data, size_t size);
  void vga_term_write_cstring(const char* data);
  void vga_term_scroll_lines(size_t line);
  void vga_term_clear_lines(size_t line);

#ifdef __cplusplus
}
#endif

#endif
