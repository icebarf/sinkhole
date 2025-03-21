#include <cstdint>
#include <kernel/arch/i386/mem/multiboot.h>
#include <kernel/serial>
#include <kernel/utils/log>
#include <kernel/vga_tty>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static logger klog;
using comp = logger::Components;

static constexpr int MAX_WORKABLE_MEM_ENTRIES = 20;

extern bool __stack_guard_initialized;

void
output_init()
{
  // initialize VGA and Serial components
  vga_term_init();
  klog.write(comp::VGA, "initialized");

  int v = serial_init();
  if (v >= 1) {
    klog.write(comp::Serial, "initialization failure: code: %d", v);
    abort();
  }
  klog.write(comp::Serial, "initialized: code: %d", v);
}

void
stack_guard_check_init()
{

  if (!__stack_guard_initialized) {
    klog.write(comp::StackGuard, "uninitialized");
  }
  klog.write(comp::StackGuard, "was intialized at boot");
}

struct memory_region
{
  uint64_t base_addr;
  uint64_t len;
  uint64_t size;
};

void
kernel_memory_init(memory_region* workable,
                   multiboot_info_t* mbi,
                   unsigned int mbmagic)
{
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

  int entries = 0;
  for (unsigned int i = 0; i < mbi->mmap_length; i++) {
    multiboot_memory_map_t* mmm_table =
      (multiboot_memory_map_t*)(mbi->mmap_addr + i);
    // printf("ID: %d | Start Addr: %llx | Length: %llx | Size: %x | Type:
    // %d\n",
    //        i,
    //        mmm_table->addr,
    //        mmm_table->len,
    //        mmm_table->size,
    //        mmm_table->type);
    if (mmm_table->type == MULTIBOOT_MEMORY_AVAILABLE) {
      workable[entries++] = { .base_addr = mmm_table->addr,
                              .len = mmm_table->len,
                              .size = mmm_table->size };
      // printf("Memory block available!\n");
    }
  }
}

uint64_t
kernel_memory_total_available(memory_region* memory)
{
  uint64_t total = 0;
  for (int i = 0; i < MAX_WORKABLE_MEM_ENTRIES; i++) {
    total += memory[i].len;
  }
  return total;
}

memory_region*
kernel_memory_find_largest(memory_region* memory)
{
  memory_region* largest = nullptr;
  largest = &memory[0];
  for (int i = 0; i < MAX_WORKABLE_MEM_ENTRIES; i++) {
    if (largest->len < memory[i].len)
      largest = &memory[i];
  }
  return largest;
}

extern "C" void
kernel_main(multiboot_info_t* mb_info, unsigned int mb_magic)
{
  memory_region memory[MAX_WORKABLE_MEM_ENTRIES];
  memset(&memory[0], 0, MAX_WORKABLE_MEM_ENTRIES * sizeof(*memory));

  output_init();
  stack_guard_check_init();

  kernel_memory_init(memory, mb_info, mb_magic);
  uint64_t mem_available = kernel_memory_total_available(memory);
  klog.write(comp::RAM, "Total Size: %lx Bytes", mem_available);
  klog.write(comp::RAM, "Total Size: %ld MiB", (mem_available / (1024 * 1024)));

  memory_region* largest;
  largest = kernel_memory_find_largest(memory);
  klog.write(comp::RAM, "Largest Workable Region found");
  if (largest->len > 0) {
    klog.write(comp::RAM, "Base: %llx", largest->base_addr);
    klog.write(comp::RAM, "Length: %ld MiB", largest->len / (1024 * 1024));
  }
  // klog.write(comp::RAM,
  //            "Base: %llx | Length: %d | Size: %lld GiB",
  //            largest->base_addr,
  //            largest->len,
  //            largest->size / (1024 * 1024 * 1024));
  klog.write(comp::Kernel, "Hello!");

  abort();
}
