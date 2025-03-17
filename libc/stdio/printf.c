#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

static int
__putstring(const char* str)
{
  int counter = 0;
  while (*str) {
    putchar(*str);
    counter++;
    str++;
  }

  return counter;
}

static int
__numlen(long num, long base)
{
  int len = 0;
  while (num) {
    num /= base;
    len++;
  }
  return len;
}

static char*
__iota_num_string(long num, int base)
{
  const char chars[] = "0123456789ABCDEF";
  static char buf[100] = { 0 }; // more than enough
  memset(buf, 0, sizeof(buf));
  int cursor = __numlen(num, base);

  long tmp = num;
  while (tmp) {
    long t = tmp % base;
    buf[--cursor] = chars[t];
    tmp /= base;
  }

  return buf;
}

int
printf(const char* __restrict fmt, ...)
{
  int cursor = 0;

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
          case 'l': {
            fmt++;
            if (*fmt == 'd') {
              long a = va_arg(args, long);
              int l = __numlen(a, 10);
              const char* num = __iota_num_string(a, 10);
              __putstring(num);
              cursor += l;
            } else if (*fmt == 'x') {
              long a = va_arg(args, long);
              int l = __numlen(a, 16);
              putchar('0');
              putchar('x');
              const char* num = __iota_num_string(a, 16);
              __putstring(num);
              cursor += l;
            }
            break;
          }
          case 'd': {
            long a = va_arg(args, long);
            int l = __numlen(a, 10);
            const char* num = __iota_num_string(a, 10);
            __putstring(num);
            cursor += l;
            break;
          }
          case 'x': {
            long a = va_arg(args, long);
            int l = __numlen(a, 16);
            putchar('0');
            putchar('x');
            const char* num = __iota_num_string(a, 16);
            __putstring(num);
            cursor += l;
            break;
          }
          case 'c': {
            char c = (char)(va_arg(args, int));
            putchar(c);
            break;
          }
          case 's': {
            const char* str = va_arg(args, const char*);
            int l = __putstring(str);
            cursor += l;
            break;
          }
        }
        break;
      }
      default:
        putchar(*fmt);
    }
    fmt++;
  }

  va_end(args);
  return cursor;
}
