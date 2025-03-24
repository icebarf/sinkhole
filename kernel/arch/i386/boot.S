.set ALIGN, 1<<0
.set MEMINFO, 1<<1
.set FLAGS, ALIGN | MEMINFO
.set MAGIC, 0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bss
.align 16
stack_bottom:
  .skip 16386
stack_top:

.align 8
.global gdt_begin
.global gdt_end
# 32 entries
gdt_begin:
  .skip 8 * 32
gdt_end:

.data
.global gdt_size
.align 4
gdt_size:
  .int gdt_end - gdt_begin
gdt:
  .word gdt_end - gdt_begin
  .long gdt_begin

.section .text
.global _start
.type _start, @function
_start:
  movl $stack_top, %esp
  # These two are filled up by GRUB according to Multiboot spec
  # according to system-V abi, we push these two registers and 
  # treat them as arguments to `kernel_main`
  # TODO: do above more elegantly as the functions before 
  #       kernel_main may hijack.
  push %eax
  push %ebx
  call _init
  call stack_guard_init
  # call kernel_create_gdt
  # call kernel_load_gdt
  call kernel_main

  cli
1:hlt
  jmp 1b

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

.size _start, . - _start