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

.size _start, . - _start