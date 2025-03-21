#include <kernel/utils/log.hpp>

const char*
logger::get_component_string(Components c)
{
  static const char*(
    strings[Components::Count]) = { [Components::Kernel] = "Kernel:",
                                    [Components::VGA] = "VGA:",
                                    [Components::RAM] = "RAM:",
                                    [Components::Serial] = "Serial:",
                                    [Components::StackGuard] = "Stack Guard:" };
  return strings[c];
}