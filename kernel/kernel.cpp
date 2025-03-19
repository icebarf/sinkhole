#include <kernel/log>
#include <kernel/serial>
#include <kernel/vga_tty>

using comp = logger::Components;

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

  klog.write(comp::Kernel, "Hello!");

  klog.write(comp::Kernel, "hlt...");
}
