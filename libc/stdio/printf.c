#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

static int
__putstring(const char* str)
{
  int len = (int)strlen(str);
  for (int i = 0; i < len; i++)
    putchar(str[i]);
  return len;
}

static int
__numlen(long long num, long base)
{
  int len = 0;
  while (num) {
    num /= base;
    len++;
  }
  return len;
}

static const char*
__iota_num_string(long long num, long base)
{
  if (num == 0)
    return "0";
  const char chars[] = "0123456789ABCDEF";
  static char buf[100] = { 0 }; // more than enough
  int cursor = __numlen(num, base);
  buf[cursor] = 0;

  long long tmp = num;
  while (tmp) {
    long long t = tmp % base;
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
          LONG_LONG:
            fmt++;
            if (*fmt == 'd') {
              long long a = va_arg(args, long long);
              int l = __numlen(a, 10);
              const char* num = __iota_num_string(a, 10);

              __putstring(num);
              cursor += l;
            } else if (*fmt == 'x') {
              long long a = va_arg(args, long long);
              int l = __numlen(a, 16);
              const char* num = __iota_num_string(a, 16);

              putchar('0');
              putchar('x');
              __putstring(num);
              cursor += l + 2; // account for two putchar
            } else if (*fmt == 'l')
              goto LONG_LONG;
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
            const char* num = __iota_num_string(a, 16);

            putchar('0');
            putchar('x');
            __putstring(num);
            cursor += l + 2; // account for two putchar
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

            int l = __putstring(str);
            cursor += l;
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
  return cursor;
}
