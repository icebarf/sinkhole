#include <cstdint>
#include <kernel/utils/log>
#include <kernel/utils/random>

#include <stdlib.h>

uintptr_t __stack_chk_guard = 0;

static bool __already_init = false;     // local book-keeping
bool __stack_guard_initialized = false; // for visibility outside

logger klog;
using comp = logger::Components;

extern "C"
{
  void stack_guard_init()
  {
    if (__already_init) {
      klog.write(comp::StackGuard,
                 "Attempting to reinitialize stack guard magic number");
      klog.write(
        comp::StackGuard,
        "Assuming violation in code either by attacker or from programmer");
      abort();
    }

    __stack_chk_guard = kgen_random_32b();
    __already_init = true;
    __stack_guard_initialized = true;
  }

  void __stack_chk_fail()
  {
    klog.write(comp::StackGuard, "Stack Smashing detected...");
    klog.write(
      comp::StackGuard,
      "Assuming violation in code either by attacker or from programmer");

    abort();
  }
}