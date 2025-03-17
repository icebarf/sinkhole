#include <cstdint>

#include <kernel/serial>

uint16_t* SERIAL_BASE_ADDR = (uint16_t*)0x0400;
uint16_t* SERIAL_COM_1 = (uint16_t*)SERIAL_BASE_ADDR + 0;
uint16_t* SERIAL_COM_2 = (uint16_t*)SERIAL_BASE_ADDR + 2;
uint16_t* SERIAL_COM_3 = (uint16_t*)SERIAL_BASE_ADDR + 4;
uint16_t* SERIAL_COM_4 = (uint16_t*)SERIAL_BASE_ADDR + 6;

static void
outb(uint16_t* port, uint8_t byte)
{
  (void)port;
  (void)byte;
  asm volatile("movw %%di, %%dx\n"
               "movw %%si, %%ax\n"
               "outw %%ax, %%dx\n"
               :
               :
               : "dx", "ax");
}

static uint16_t
inb(uint16_t* port)
{
  (void)port;
  uint16_t ret = 255;
  asm volatile("movw %%di, %%dx\n"
               "inw %%dx, %%ax\n"
               "mov %%ax, %0\n"
               : "+r"(ret)
               :
               : "dx", "ax");

  return ret;
}

#define PORT (uint16_t*)0x3f8 // COM1

int
init_serial()
{
  outb(PORT + 1, 0x00); // Disable all interrupts
  outb(PORT + 3, 0x80); // Enable DLAB (set baud rate divisor)
  outb(PORT + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
  outb(PORT + 1, 0x00); //                  (hi byte)
  outb(PORT + 3, 0x03); // 8 bits, no parity, one stop bit
  outb(PORT + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
  outb(PORT + 4, 0x0B); // IRQs enabled, RTS/DSR set
  outb(PORT + 4, 0x1E); // Set in loopback mode, test the serial chip
  outb(PORT + 0, 0xAE); // Test serial chip (send byte 0xAE and check if serial
                        // returns same byte)

  // Check if serial is faulty (i.e: not same byte as sent)
  int rval;
  if ((rval = inb(PORT + 0)) != 0xAE) {
    return rval;
  }

  // If serial is not faulty set it in normal operation mode
  // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
  outb(PORT + 4, 0x0F);
  return 0;
}
