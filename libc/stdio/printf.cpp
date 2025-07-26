#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <type_traits>

static unsigned long
putstring(const char* str)
{
  unsigned long len = strlen(str);
  for (unsigned int i = 0; i < len; i++)
    putchar(str[i]);
  return len;
}

template<typename T>
static int
numlen(T num, unsigned long base)
{
  if (num == 0)
    return 1;
  int len = 0;
  while (num) {
    num /= base;
    len++;
  }
  return len;
}

template<typename T>
static const char*
iota_num_string(T num, unsigned long base)
{
  if (num == 0)
    return "0";

  const char chars[] = "0123456789ABCDEF";
  static char buf[256] = { 0 }; // more than enough
  memset(buf, 0, sizeof(buf));
  bool is_negative = false;

  if (num < 0) {
    num = -num;
    is_negative = true;
  }

  int cursor = numlen<T>(num, base);
  if (is_negative)
    cursor += 1; // account for '-' and 0 byte
  buf[cursor] = 0;

  T tmp = num;
  while (tmp) {
    T t = tmp % base;
    buf[--cursor] = chars[t];
    tmp /= base;
  }
  if (is_negative)
    buf[--cursor] = '-';

  return buf;
}

constexpr int base_dec = 10;
constexpr int base_hex = 16;

int
printf(const char* __restrict fmt, ...)
{
  unsigned long cursor = 0;

  const auto& print_int = [&cursor](auto arg, unsigned int base) {
    const char* num = iota_num_string<decltype(arg)>(arg, base);
    size_t len = strlen(num);
    putstring(num);
    cursor += len;
  };

  int length = (int)strlen(fmt);
  if (!length)
    return 0;

  va_list args;
  va_start(args, fmt);

  // helpers

  const auto& handle_generic_int = [&args, &print_int]<typename T, int base>() {
    T arg = va_arg(args, T);
    print_int(arg, base);
  };

  const auto& handle_all_ints =
    // `&args` is captured but further captured and used in handle_generic_int
    [&args, &fmt, &handle_generic_int]<typename T>() {
      using Type = T;
      using uType = std::make_unsigned_t<T>;

      switch (*fmt) {
        case 'u': {
          handle_generic_int.template operator()<uType, base_dec>();
        } break;

        case 'i':
        case 'd': {
          handle_generic_int.template operator()<Type, base_dec>();
        } break;

        case 'x': {
          handle_generic_int.template operator()<uType, base_hex>();
        } break;
      }
    };

  // actual printf implementation

  while (*fmt) {
    switch (*fmt) {
      case '%': {
        fmt++;
        switch (*fmt) {
          case '%': {
            putchar('%');
            cursor++;
          } break;

          // LONG INT
          case 'l': {
            fmt++;
            switch (*fmt) {
              // LONG LONG INT
              case 'l': {
                fmt++;
                handle_all_ints.template operator()<long long>();
              } // case 'l':

              default:
                break;
            }
            default:
              handle_all_ints.template operator()<long>();
              break;
          }

          case 'z': {
            fmt++;
            switch (*fmt) {
              case 'x':
              case 'X': {
                handle_generic_int.template operator()<size_t, base_hex>();
              } break;

              case 'u': {
                handle_generic_int.template operator()<size_t, base_dec>();
              } break;
            }
          } break;

          case 'u':
          case 'i':
          case 'd':
          case 'X':
          case 'x': {
            handle_all_ints.template operator()<int>();
          }

          case 'c': {
            char c = (char)(va_arg(args, int));
            putchar(c);
            cursor++;
            break;
          }

          case 's': {
            const char* str = va_arg(args, const char*);
            unsigned long len = putstring(str);
            cursor += len;
            break;
          }

          case 'p': {
            handle_generic_int.template operator()<unsigned long, base_hex>();
            break;
          }
        }

        break;
      }
      default:
        putchar(*fmt);
        cursor++;
    }
    fmt++;
  }

  va_end(args);
  return (int)cursor;
}
