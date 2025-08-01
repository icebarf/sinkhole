#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <concepts>
#include <type_traits>

static unsigned long
putstring(const char* str)
{
  unsigned long len = strlen(str);
  for (unsigned int i = 0; i < len; i++)
    putchar(str[i]);
  return len;
}

template<std::integral T>
static int
numlen(T num, T base)
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

template<std::integral T>
static const char*
iota_num_string(T num, T base)
{
  if (num == 0)
    return "0";

  const char chars[] = "0123456789ABCDEF";
  static char buf[256] = { 0 }; // more than enough
  memset(buf, 0, sizeof(buf));

  bool is_negative = false;
  using unsignedT = std::make_unsigned_t<T>;
  unsignedT tmp;
  if constexpr (std::is_signed_v<T>) {
    if (num < 0) {
      is_negative = true;
      tmp = static_cast<unsignedT>(0) - static_cast<unsignedT>(num);
    } else {
      tmp = static_cast<unsignedT>(num);
    }
  } else {
    tmp = num;
  }

  unsignedT ubase = static_cast<unsignedT>(base);

  int cursor = numlen(tmp, ubase);
  if (is_negative)
    cursor += 1; // account for '-'
  buf[cursor] = 0;

  while (tmp) {
    buf[--cursor] = chars[tmp % ubase];
    tmp /= ubase;
  }
  if (is_negative)
    buf[--cursor] = '-';

  return &buf[cursor];
}

constexpr int base_dec = 10;
constexpr int base_hex = 16;

int
printf(const char* __restrict fmt, ...)
{
  unsigned long cursor = 0;

  const auto& print_int = [&cursor]<std::integral T>(T arg, T base) {
    const char* num = iota_num_string<T>(arg, base);
    size_t len = putstring(num);
    cursor += len;
  };

  int length = (int)strlen(fmt);
  if (!length)
    return 0;

  va_list args;
  va_start(args, fmt);

  // helpers
  const auto& handle_generic_int = [&args, &print_int]<typename T, int base>() {
    using RawT = std::remove_cv_t<std::remove_reference_t<T>>;
    RawT arg;

    if constexpr (std::is_same_v<RawT, long long>)
      arg = va_arg(args, long long);
    else if constexpr (std::is_same_v<RawT, unsigned long long>)
      arg = va_arg(args, unsigned long long);
    else
      arg = va_arg(args, RawT);

    print_int(arg, static_cast<RawT>(base));
  };

  const auto& handle_all_ints =
    // `&args` is captured but further captured and used in handle_generic_int
    [&handle_generic_int]<std::integral T>(char fmtc) {
      using Type = T;
      using uType = std::make_unsigned_t<T>;

      switch (fmtc) {
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
            // LONG LONG INT
            if (*fmt == 'l') {
              fmt++;
              handle_all_ints.template operator()<long long>(*fmt);
              break;
            }
            handle_all_ints.template operator()<long>(*fmt);
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

              default:
                break;
            }

          } break;

          case 'u':
          case 'i':
          case 'd':
          case 'X':
          case 'x': {
            handle_all_ints.template operator()<int>(*fmt);
          } break;

          case 'c': {
            char c = (char)(va_arg(args, int));
            putchar(c);
            cursor++;
          } break;

          case 's': {
            const char* str = va_arg(args, const char*);
            unsigned long len = putstring(str);
            cursor += len;
          } break;

          case 'p': {
            handle_generic_int.template operator()<unsigned long, base_hex>();
          } break;
        }
      } break;

      default:
        putchar(*fmt);
        cursor++;
    }
    fmt++;
  }

  va_end(args);
  return (int)cursor;
}
