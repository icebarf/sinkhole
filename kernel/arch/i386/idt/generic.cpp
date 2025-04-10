
// generic exception handler
extern "C" [[noreturn]]
void
generic_exception_handler()
{
  // 腹切り
  asm volatile("cli; hlt");
  __builtin_unreachable();
}