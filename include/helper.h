#ifndef HELPER_H_
#define HELPER_H_

#include <errno.h>

typedef int Errno;

#define return_defer(value) do {result = (value); goto defer; } while (0)

#endif // HELPER_H_
