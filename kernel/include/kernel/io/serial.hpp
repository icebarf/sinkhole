#ifndef SINKHOLE_KERNEL_SERIAL_H
#define SINKHOLE_KERNEL_SERIAL_H

#ifdef __cplusplus
#include <cstdint>
#else
#include <stdint.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif
  int serial_init();

  uint8_t serial_read();

  void serial_write(int8_t byte);

#ifdef __cplusplus
}
#endif

#endif