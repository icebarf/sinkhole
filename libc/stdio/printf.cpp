#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

static unsigned long
__putstring(const char* str)
{
  unsigned long len = strlen(str);
  for (unsigned int i = 0; i < len; i++)
    putchar(str[i]);
  return len;
}

static int
__numlen_signed(long long num, unsigned long base)
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

static int
__numlen_unsigned(unsigned long long num, unsigned long base)
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

static const char*
__iota_num_string_unsigned(unsigned long long num, unsigned long base)
{
  if (num == 0)
    return "0";
  const char chars[] = "0123456789ABCDEF";
  static char buf[100] = { 0 }; // more than enough
  int cursor = __numlen_unsigned(num, base);
  buf[cursor] = 0;

  unsigned long long tmp = num;
  while (tmp) {
    unsigned long long t = tmp % base;
    buf[--cursor] = chars[t];
    tmp /= base;
  }

  return buf;
}

static const char*
__iota_num_string_signed(long long num, unsigned long base)
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

  int cursor = __numlen_signed(num, base);
  if (is_negative)
    cursor += 1; // account for '-' and 0 byte
  buf[cursor] = 0;

  long long tmp = num;
  while (tmp) {
    long long t = tmp % base;
    buf[--cursor] = chars[t];
    tmp /= base;
  }
  if (is_negative)
    buf[--cursor] = '-';

  return buf;
}

int
printf(const char* __restrict fmt, ...)
{
  unsigned long cursor = 0;

  int len = (int)strlen(fmt);
  if (!len)
    return 0;

  va_list args;
  va_start(args, fmt);

  while (*fmt) {
    switch (*fmt) {
      case '%': {
        fmt++;
        switch (*fmt) {
          // LONG INT
          case '%': {
            putchar('%');
            cursor++;
          } break;

          case 'l': {
            fmt++;
            switch (*fmt) {
              case 'u': {
                unsigned long a = va_arg(args, unsigned long);
                const char* num = __iota_num_string_unsigned(a, 10);
                size_t l = strlen(num);
                __putstring(num);
                cursor += l;
              } break;

              case 'i':
              case 'd': {
                long a = va_arg(args, long);
                const char* num = __iota_num_string_signed(a, 10);
                size_t l = strlen(num);
                __putstring(num);
                cursor += l;
              } break;

              case 'x': {
              PRINTF_POINTER_OUT:
                unsigned long a = va_arg(args, unsigned long);
                const char* num = __iota_num_string_unsigned(a, 16);
                size_t l = strlen(num);
                __putstring(num);
                cursor += l; // account for two putchar
              } break;

              // LONG LONG INT
              case 'l': {
                fmt++;

                switch (*fmt) {
                  case 'u': {
                    unsigned long long a = va_arg(args, unsigned long long);
                    const char* num = __iota_num_string_unsigned(a, 10);
                    size_t l = strlen(num);
                    __putstring(num);
                    cursor += l;
                  } break;

                  case 'i':
                  case 'd': {
                    long long a = va_arg(args, long long);
                    const char* num = __iota_num_string_signed(a, 10);
                    __putstring(num);
                    size_t l = strlen(num);
                    cursor += l;
                  } break;

                  case 'x': {
                    unsigned long long a = va_arg(args, unsigned long long);
                    const char* num = __iota_num_string_unsigned(a, 16);
                    __putstring(num);
                    size_t l = strlen(num);
                    cursor += l; // account for two putchar
                  } break;

                  default:
                    break;
                }
              }

              default:
                break;
            }
            default:
              break;
          }

          case 'z': {
            fmt++;
            switch (*fmt) {
              case 'x':
              case 'X': {
                size_t a = va_arg(args, size_t);
                const char* num = __iota_num_string_unsigned(a, 16);
                __putstring(num);
                size_t l = strlen(num);
                cursor += l;
              } break;

              case 'u': {
                size_t a = va_arg(args, size_t);
                const char* num = __iota_num_string_unsigned(a, 10);
                __putstring(num);
                size_t l = strlen(num);
                cursor += l;
              } break;
            }
          } break;

          case 'u': {
            unsigned int a = va_arg(args, unsigned int);
            const char* num = __iota_num_string_unsigned(a, 10);
            __putstring(num);
            size_t l = strlen(num);
            cursor += l;
            break;
          }

          case 'i':
          case 'd': {
            int a = va_arg(args, int);
            const char* num = __iota_num_string_signed(a, 10);
            __putstring(num);
            size_t l = strlen(num);
            cursor += l;
            break;
          }

          case 'x': {
            unsigned int a = va_arg(args, unsigned int);
            const char* num = __iota_num_string_unsigned(a, 16);
            __putstring(num);
            size_t l = strlen(num);
            cursor += l;
            break;
          }

          case 'c': {
            char c = (char)(va_arg(args, int));
            putchar(c);
            cursor++;
            break;
          }

          case 's': {
            const char* str = va_arg(args, const char*);
            unsigned long l = __putstring(str);
            cursor += l;
            break;
          }

          case 'p': {
            goto PRINTF_POINTER_OUT;
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
