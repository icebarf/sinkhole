#include <cstdint>

#include <kernel/serial>

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

// COM1 Port Address
#define COM (uint16_t* const)0x3f8

// Each of these named addresses are 8-bit registers
#define RW_BUFFER (COM + 0)
#define BAUD_DIVISOR_LOWB (COM + 0) // Accessible if LINE_CONTROL MSB is set
#define IRQ_ENABLE_REG (COM + 1)
#define BAUD_DIVISOR_HIGHB (COM + 1) // Accessible if LINE_CONTROL MSB is set
#define IRQ_IDENTIFY (COM + 2)
#define FIFO_CONTROL (COM + 2)
#define LINE_CONTROL (COM + 3) // MSB is DLAB which if set allows BAUD Control
#define MODEM_CONTROL (COM + 4)
#define LINE_STATUS (COM + 5)
#define MODEM_STATUS (COM + 6)
#define SCRATCH_REG (COM + 7)

int
init_serial()
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
