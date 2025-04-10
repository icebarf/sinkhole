#ifndef SINKHOLE_I386_IDT_H
#define SINKHOLE_I386_IDT_H

#include <stdint.h>

using idt_entry_t = uint64_t;

static constexpr idt_entry_t
idt_create_descriptor(uint32_t isr, uint8_t flags)
{
  idt_entry_t entry = 0;
  // make our life easier
  struct idt_entry_struct
  {
    uint16_t isr_low;
    uint16_t kernel_cs;
    uint8_t reserved;
    uint8_t attributes;
    uint16_t isr_high;

    constexpr idt_entry_struct()
      : isr_low{}
      , kernel_cs{}
      , reserved{}
      , attributes{}
      , isr_high{}
    {
    }

    constexpr operator idt_entry_t()
    {
      idt_entry_t e = 0;
      e |= (uint64_t)isr_low << 48;
      e |= (uint64_t)kernel_cs << 32;
      e |= (uint64_t)reserved << 24;
      e |= (uint64_t)attributes << 16;
      e |= isr_high;
      return e;
    }
  } __attribute__((packed)) descriptor;

  descriptor.isr_low = isr & 0xffff;
  descriptor.kernel_cs = 0x08; // gdt SYS_CODE segment offset
  descriptor.attributes = flags;
  descriptor.isr_high = isr >> 16;
  descriptor.reserved = 0;

  entry = static_cast<idt_entry_t>(descriptor);
  return entry;
}

extern "C" constexpr int idt_max_entries = 256;
extern uint32_t* isr_stub_table;
extern "C" idt_entry_t idt_entries[idt_max_entries] = { 0 };
extern "C" int idt_limit = (uint16_t)sizeof(idt_entry_t) * idt_max_entries - 1;

inline void
idt_create()
{
  for (int vector = 0; vector < 31; vector++) {
    idt_entries[vector] = idt_create_descriptor(isr_stub_table[vector], 0x8e);
  }
}

#endif