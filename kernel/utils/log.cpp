#include <kernel/log>

const char*
logger::get_component_string(Components c)
{
  static const char*(
    strings[Components::Count]) = { [Components::Kernel] = "Kernel:",
                                    [Components::VGA] = "VGA:",
                                    [Components::Serial] = "Serial:" };
  return strings[c];
}