#include <kernel/utils>

uint8_t
kgen_random_8b()
{
  uint8_t ret;
  asm volatile("rdrand %0\n" : "=a"(ret));
  return ret;
}

uint16_t
kgen_random_16b()
{
  uint16_t ret;
  asm volatile("rdrand %0\n" : "=a"(ret));
  return ret;
}

uint32_t
kgen_random_32b()
{
  uint32_t ret;
  asm volatile("rdrand %0\n" : "=a"(ret));
  return ret;
}