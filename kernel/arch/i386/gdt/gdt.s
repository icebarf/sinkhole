# From kernel/arch/i386/gdt/gdt.hpp
.section .data
.align 4
gdt:
  .word gdt_size
  .long gdt_entries

.section .text
.global kernel_load_gdt
.type kernel_load_gdt, @function
kernel_load_gdt:
  cli
  lgdt gdt
  jmp $0x08,$.reload_registers # Reloads CS with that address
.reload_registers:
   mov $0x10, %ax
   mov %ax, %ds
   mov %ax, %es
   mov %ax, %fs
   mov %ax, %gs
   mov %ax, %ss
  ret
