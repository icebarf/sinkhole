#ifndef _SINKHOLE_LIBC_STRING_H
#define _SINKHOLE_LIBC_STRING_H

#include <sys/cdefs.h>

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif
  /* String functions */
  size_t __attribute__((warn_unused_result)) strlen(const char* str);

  int __attribute__((warn_unused_result)) strncmp(const char* lhs,
                                                  const char* rhs,
                                                  size_t n);

  char* strncpy(char* __restrict dest, const char* __restrict src, size_t n);

  /* Memory functions*/
  void* memcpy(void* __restrict dest, const void* __restrict src, size_t n);

  void* memmove(void* dest, const void* src, size_t n);

  int memcmp(const void* lhs, const void* rhs, size_t count);

  void* memset(void* bufptr, int value, size_t size);

#ifdef __cplusplus
}
#endif

#endif