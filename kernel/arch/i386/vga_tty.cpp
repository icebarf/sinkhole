#include <cstddef>
#include <cstdint>
#include <string.h>

#include <kernel/vga_tty>

#include "vga.h"

constexpr std::size_t VGA_WIDTH = 80;
constexpr std::size_t VGA_HEIGHT = 25;
static uint16_t* const VGA_MEMORY = (uint16_t*)0xB8000;

static std::size_t vga_term_row;
static std::size_t vga_term_column;
static uint8_t vga_term_color;
static uint16_t* vga_term_buffer;

void
vga_term_init()
{
  vga_term_row = 0;
  vga_term_column = 0;
  vga_term_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
  vga_term_buffer = VGA_MEMORY;
  for (size_t y = 0; y < VGA_HEIGHT; y++) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      const size_t index = y * VGA_WIDTH + x;
      vga_term_buffer[index] = vga_entry(' ', vga_term_color);
    }
  }
}

void
vga_term_setcolor(uint8_t color)
{
  vga_term_color = color;
}

void
vga_term_putentryat(unsigned char c, uint8_t color, size_t x, size_t y)
{
  const size_t index = y * VGA_WIDTH + x;
  vga_term_buffer[index] = vga_entry(c, color);
}

void
vga_term_clear_lines(size_t line)
{
  for (size_t y = VGA_HEIGHT - line; y < VGA_WIDTH; y++) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      vga_term_putentryat(' ', vga_term_color, y, x);
    }
  }
}

void
vga_term_scroll_lines(size_t line)
{
  for (size_t y = line - 1; y < VGA_HEIGHT; y++)
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      const size_t index = ((y - (line - 1)) * VGA_WIDTH) + x;
      const size_t index_next = (y + 1) * VGA_WIDTH + x;
      vga_term_buffer[index] = vga_term_buffer[index_next];
    }
}

void
vga_term_putchar(const char c)
{
  if (c == '\n') {
    vga_term_row++;
    if (vga_term_row >= VGA_HEIGHT) {
      vga_term_scroll_lines(1);
      vga_term_clear_lines(1);
      vga_term_row--;
    }
    vga_term_column = 0;
    return;
  }

  vga_term_putentryat(c, vga_term_color, vga_term_column, vga_term_row);
  vga_term_column++;
  if (vga_term_column >= VGA_WIDTH) {
    vga_term_column = 0;
    vga_term_row++;
    if (vga_term_row >= VGA_HEIGHT) {
      vga_term_scroll_lines(1);
      vga_term_clear_lines(1);
      vga_term_row--;
    }
  }
}

void
vga_term_write(const char* data, std::size_t size)
{
  for (size_t i = 0; i < size; i++) {
    vga_term_putchar(data[i]);
  }
}

void
vga_term_write_cstring(const char* data)
{
  vga_term_write(data, strlen(data));
}
