#include <kernel/log>
#include <kernel/serial>
#include <kernel/vga_tty>

extern "C" void
kernel_main()
{
  logger klog;

  // initialize VGA and Serial components
  vga_term_init();
  klog.write(logger::Components::VGA, "initialized");

  int v = serial_init();
  if (v >= 1) {
    klog.write(
      logger::Components::Serial, "initialization failure: code: %d", v);
    return;
  }
  klog.write(logger::Components::Serial, "initialized: code: %d", v);

  klog.write(logger::Components::Kernel, "Hello!");

  klog.write(logger::Components::Kernel, "hlt...");
}
