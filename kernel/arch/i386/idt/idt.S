.section .data
.align 4
idt:
  .word idt_limit
  .long idt_entries

.section .text
.global kernel_load_idt
.type kernel_load_idt, @function
kernel_load_idt:
  lidt idt
  sti
  ret

.macro isr_err_stub vector
isr_stub_\vector :
  call generic_exception_handler
  iret
.endm

.macro isr_no_err_stub vector
isr_stub_\vector :
  call generic_exception_handler
  iret
.endm

isr_no_err_stub 000
isr_no_err_stub 001
isr_no_err_stub 002
isr_no_err_stub 003
isr_no_err_stub 004
isr_no_err_stub 005
isr_no_err_stub 006
isr_no_err_stub 007
isr_err_stub    008
isr_no_err_stub 009
isr_err_stub    010
isr_err_stub    011
isr_err_stub    012
isr_err_stub    013
isr_err_stub    014
isr_no_err_stub 015
isr_no_err_stub 016
isr_err_stub    017
isr_no_err_stub 018
isr_no_err_stub 019
isr_no_err_stub 020
isr_no_err_stub 021
isr_no_err_stub 022
isr_no_err_stub 023
isr_no_err_stub 024
isr_no_err_stub 025
isr_no_err_stub 026
isr_no_err_stub 027
isr_no_err_stub 028
isr_no_err_stub 029
isr_err_stub    030
isr_no_err_stub 031

.global isr_stub_table
isr_stub_table:
  .irp vector,000,001,002,003,004,005,006,007,008,009,010,011,012,013,014,015,\
              016,017,018,019,020,021,022,023,024,025,026,027,028,029,030,031
  .long isr_stub_\vector
  .endr