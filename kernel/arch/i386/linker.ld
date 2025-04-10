ENTRY(_start)

SECTIONS
{
# . = 1M; doesn't work because some parts seemingly get pushed out
# INVESTIGATE!!!!!!!!!!!!!!!  
  . = 0M;

  .text BLOCK(4K) : ALIGN(4K)
  {
    *(.multiboot)
    *(.text)
  }

  .rodata BLOCK(4K) : ALIGN(4K)
  {
    *(.rodata)
  }

  .data BLOCK(4K) : ALIGN(4K)
  {
    *(.data)
  }

  .bss BLOCK(4K) : ALIGN(4K)
  {
    *(COMMON)
    *(.bss)
  }

  kernel_end = (. + 3) & ~3;
}