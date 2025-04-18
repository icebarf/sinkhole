#ifndef SINKHOLE_LOG_H
#define SINKHOLE_LOG_H

#include <stdio.h>

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

  template<typename... T>
  void write(Components c, T... args)
  {
    printf("%s ", get_component_string(c));
    printf(args...);
    putchar('\n');
  }
};

#endif