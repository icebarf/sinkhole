#include <kernel/serial>
#include <kernel/utils/log>
#include <kernel/vga_tty>

using comp = logger::Components;

extern bool __stack_guard_initialized;

extern "C" void
kernel_main()
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

  if (!__stack_guard_initialized) {
    klog.write(comp::StackGuard, "uninitialized");
  }
  klog.write(comp::StackGuard, "was intialized at boot");

  klog.write(comp::Kernel, "Hello!");

  klog.write(comp::Kernel, "hlt...");
}
