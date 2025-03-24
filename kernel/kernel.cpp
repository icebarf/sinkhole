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
extern int gdt_size;
extern uintptr_t gdt_begin;
extern uintptr_t gdt_end;
extern uintptr_t kernel_end;
extern "C" void
kernel_load_gdt();

// symbols for local use
static constexpr int MAX_WORKABLE_MEM_ENTRIES = 20;
static long int gdt_entries_written = 0;

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

extern "C" void
kernel_create_gdt()
{
  klog.write(comp::Kernel, "GDT Table Located at 0x%p", &gdt_begin);
  uint64_t* gdt = (uint64_t*)&gdt_begin;

  uint64_t entry_cs_sys = create_descriptor(0, 0x000FFFFF, (GDT_CODE_PL0));
  uint64_t entry_ds_sys = create_descriptor(0, 0x000FFFFF, (GDT_DATA_PL0));
  uint64_t entry_cs_usr = create_descriptor(0, 0x000FFFFF, (GDT_CODE_PL3));
  uint64_t entry_ds_usr = create_descriptor(0, 0x000FFFFF, (GDT_DATA_PL3));

  gdt[0] = 0;
  gdt[1] = entry_cs_sys;
  gdt[2] = entry_ds_sys;
  gdt[3] = entry_cs_usr;
  gdt[4] = entry_ds_usr;
  gdt_entries_written += 5;
  klog.write(comp::Kernel, "Entries filled into GDT");
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
  klog.write(comp::RAM, "Total Size: 0x%lx Bytes", mem_available);
  klog.write(comp::RAM, "Total Size: %lu MiB", (mem_available / (1024 * 1024)));

  memory_region* largest;
  largest = kernel_memory_find_largest(memory);
  klog.write(comp::RAM, "Largest Workable Region found");
  klog.write(comp::RAM,
             "Base: 0x%llx | Length: %llu MiB | Size: 0x%x",
             largest->base_addr,
             largest->len / (1024 * 1024),
             largest->size);

  klog.write(comp::RAM, "kernel_end at 0x%lx", &kernel_end);

  kernel_create_gdt();
  kernel_load_gdt();
  klog.write(
    comp::Kernel, "Loaded GDT Register with table located at 0x%p", &gdt_begin);
  klog.write(comp::Kernel, "GDT Size: %u Bytes", gdt_size);
  klog.write(
    comp::Kernel, "GDT Entries Possible: %u", gdt_size / sizeof(uint64_t));
  klog.write(
    comp::Kernel, "Current GDT Entries count: %u", gdt_entries_written);

  klog.write(comp::Kernel, "Hello!");

  abort();
}
