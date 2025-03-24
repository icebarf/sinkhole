#include <kernel/arch/i386/gdt/gdt.hpp>
#include <kernel/arch/i386/mem/multiboot.h>
#include <kernel/io/serial.hpp>
#include <kernel/io/vga_tty.hpp>
#include <kernel/utils/log.hpp>
#include <kernel/utils/random.hpp>

#include <cstdint>
#include <stdlib.h>
#include <string.h>

// external symbols
extern bool __stack_guard_initialized;
extern uintptr_t gdt_begin;
extern uint32_t gdt_canary;
extern uintptr_t kernel_end;
extern "C" void
kernel_load_gdt();

// symbols for local use
static uint32_t __local_gdt_canary = 0;
static constexpr int MAX_WORKABLE_MEM_ENTRIES = 20;

// logging
static logger klog;
using comp = logger::Components;

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
  uint32_t size;
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
    if (mmm_table->type == MULTIBOOT_MEMORY_AVAILABLE) {
      workable[entries++] = { .base_addr = mmm_table->addr,
                              .len = mmm_table->len,
                              .size = mmm_table->size };
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

void
kernel_create_gdt()
{
  gdt_canary = kgen_random_32b();
  __local_gdt_canary = gdt_canary;

  uint64_t* gdt = (uint64_t*)gdt_begin;

  uint64_t entry_cs_sys =
    create_descriptor(0x000000, 0x000FFFFF, (GDT_CODE_PL0));
  uint64_t entry_ds_sys =
    create_descriptor(0x000000, 0x000FFFFF, (GDT_DATA_PL0));
  uint64_t entry_cs_usr =
    create_descriptor(0x000000, 0x000FFFFF, (GDT_CODE_PL3));
  uint64_t entry_ds_usr =
    create_descriptor(0x000000, 0x000FFFFF, (GDT_DATA_PL3));

  printf("GDT Entries Generated:\n");
  printf("%llx\n", entry_cs_sys);
  printf("%llx\n", entry_ds_sys);
  printf("%llx\n", entry_cs_usr);
  printf("%llx\n", entry_ds_usr);

  gdt[0] = 0;
  gdt[1] = entry_cs_sys;
  gdt[2] = entry_ds_sys;
  gdt[3] = entry_cs_usr;
  gdt[4] = entry_ds_usr;

  printf("GDT Entries Written to pre-allocated table at: %x\n", &gdt_begin);
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
  klog.write(comp::RAM,
             "Base: %llx | Length: %lld MiB | Size: %x",
             largest->base_addr,
             largest->len / (1024 * 1024),
             largest->size);

  klog.write(comp::RAM, "kernel_end at %x", &kernel_end);

  kernel_create_gdt();
  kernel_load_gdt();
  klog.write(comp::Kernel, "loaded GDT Register with %x", &gdt_begin);

  klog.write(comp::Kernel, "Hello!");

  abort();
}
