#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static int
numlen(long num, long base)
{
  int len = 0;
  while (num) {
    num /= base;
    len++;
  }
  return len;
}

void
iota_write(long num, int base)
{
  const char chars[] = "0123456789ABCDEF";

  long tmp = num;
  while (tmp) {
    long t = tmp % base;
    putchar(chars[t]);
    tmp /= base;
  }
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
              int l = numlen(a, 10);
              iota_write(a, 10);
              cursor += l;
            } else if (*fmt == 'x') {
              long a = va_arg(args, long);
              int l = numlen(a, 16);
              putchar('0');
              putchar('x');
              iota_write(a, 16);
              cursor += l;
            }
            break;
          }
          case 'd': {
            int a = va_arg(args, int);
            int l = numlen(a, 10);
            iota_write(a, 10);
            cursor += l;
            break;
          }
          case 'x': {
            int a = va_arg(args, int);
            int l = numlen(a, 16);
            putchar('0');
            putchar('x');
            iota_write(a, 16);
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
            int l = (int)strlen(str);
            while (*str) {
              putchar(*str);
              str++;
            }
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
