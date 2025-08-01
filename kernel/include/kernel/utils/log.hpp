#ifndef SINKHOLE_LOG_H
#define SINKHOLE_LOG_H

#include <stdio.h>
#include <utility>

struct logger
{
  enum Components
  {
    Kernel,
    VGA,
    RAM,
    Serial,
    StackGuard,
    Count = 5
  };

  const char* get_component_string(Components c);

  template<typename... Args>
  void write(Components c, const char* fmt, Args&&... args)
  {
    printf("%s ", get_component_string(c));
    printf(fmt, std::forward<Args>(args)...);
    putchar('\n');
  }
};

#endif