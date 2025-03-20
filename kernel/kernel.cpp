#include <cstdint>
#include <kernel/arch/i386/mem/multiboot.h>
#include <kernel/serial>
#include <kernel/utils/log>
#include <kernel/vga_tty>
#include <stdlib.h>

using comp = logger::Components;

extern bool __stack_guard_initialized;

extern "C" void
kernel_main(multiboot_info_t* mbi, unsigned int mbmagic)
{
  logger klog;

  // initialize VGA and Serial components
  vga_term_init();
  klog.write(comp::VGA, "initialized");

  int v = serial_init();
  if (v >= 1) {
    klog.write(comp::Serial, "initialization failure: code: %d", v);
    return;
  }
  klog.write(comp::Serial, "initialized: code: %d", v);

  // Check Memory related shit
  if (mbmagic != MULTIBOOT_BOOTLOADER_MAGIC) {
    klog.write(comp::Kernel, "Boot magic invalid!");
    abort();
  }

  if (!((mbi->flags >> 6) & 0x1)) {
    klog.write(comp::Kernel,
               "Bootloader did not pass any valid mmap information!");
    abort();
  }

  for (unsigned int i = 0; i < mbi->mmap_length; i++) {
    multiboot_memory_map_t* mmm_table =
      (multiboot_memory_map_t*)(mbi->mmap_addr + i);
    printf("Start Addr: %llx | Length: %llx | Size: %x | Type: %d\n",
           mmm_table->addr,
           mmm_table->len,
           mmm_table->size,
           mmm_table->type);
    if (mmm_table->type == MULTIBOOT_MEMORY_AVAILABLE) {
      printf("Memory block available!\n");
    }
  }

  if (!__stack_guard_initialized) {
    klog.write(comp::StackGuard, "uninitialized");
  }
  klog.write(comp::StackGuard, "was intialized at boot");

  klog.write(comp::Kernel, "Hello!");

  abort();
}
