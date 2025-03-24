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
gdt_canary:
  .long 0
# 10 entries
gdt_begin:
  .skip 8 * 10
.global gdt_canary
.global gdt_begin

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
  call kernel_main

  cli
1:hlt
  jmp 1b

.global kernel_load_gdt
.type kernel_load_gdt, @function
kernel_load_gdt:
  lgdt gdt_begin
  ret

.size _start, . - _start