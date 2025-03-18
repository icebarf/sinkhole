#include <cstdint>
#include <kernel/serial>
#include <kernel/vga_tty>

#include "serial.hpp"

static uint8_t
inb(uint16_t* const port)
{
  uint8_t ret = 255;
  // SEE: https://gcc.gnu.org/onlinedocs/gcc/Machine-Constraints.html
  // section: x86 family—config/i386/constraints.md
  // translates to `inb dx, al`
  asm volatile("inb %[_port], %[_ret]\n"
               : [_ret] "=a"(ret)
               : [_port] "d"(port)
               :);

  return ret;
}

static void
outb(uint16_t* const port, uint8_t byte)
{
  // translates to `outb al, dx`
  asm volatile("outb %[_byte], %[_port]\n"
               :
               : [_port] "d"(port), [_byte] "a"(byte)
               :);
}

/* SEE: serial.hpp for information about these names */
int
serial_init()
{
  // clang-format off
  /* See
   * https://wiki.osdev.org/Serial_Ports#Programming_the_Serial_Communications_Port
   * for what the magic values mean when setting up various registers
   */
  outb(IRQ_ENABLE_REG,      0b0);
  outb(LINE_CONTROL,        0b1000'0000); // Read comment above
  outb(BAUD_DIVISOR_LOWB,   0b0000'0011); // Set divisor to 3 - 38400 baud
  outb(BAUD_DIVISOR_HIGHB,  0b0);
  outb(LINE_CONTROL,        0b0000'0011); // 8 bits, no parity, one stop bit
  outb(FIFO_CONTROL,        0b1100'0111); // Enable FIFO, clear them, with 14-byte threshold
  outb(MODEM_CONTROL,       0b0000'1011); // IRQs enabled, RTS/DSR set
  outb(MODEM_CONTROL,       0b0001'1110); // Set in loopback mode, test the serial chip
  // clang-format on

  // Test serial chip (send 0xAE and check if it returns same byte)
  outb(RW_BUFFER, 0xAE);

  // Check if serial is faulty (i.e: not same byte as sent)
  int rval;
  if ((rval = inb(RW_BUFFER)) != 0xAE) {
    return rval;
  }

  // If serial is not faulty set it in normal operation mode
  // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
  outb(MODEM_CONTROL, 0b0000'1111);
  return 0;
}

static bool
check_serial_recvd()
{
  return inb(LINE_STATUS) & 0b0000'0001;
}

static bool
check_serial_sendable()
{
  return inb(LINE_STATUS) & 0b0010'0000;
}

uint8_t
serial_read()
{
  while (check_serial_recvd() != true)
    ;

  return inb(RW_BUFFER);
}

void
serial_write(int8_t byte)
{
  while (check_serial_sendable() != true)
    ;

  outb(RW_BUFFER, (uint8_t)byte);
}