#ifndef SINKHOLE_SERIAL_INTERNAL_H
#define SINKHOLE_SERIAL_INTERNAL_H

// COM1 Port Address
#define COM (uint16_t*)0x3f8

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

#endif