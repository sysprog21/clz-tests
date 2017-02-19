#ifndef CLZ2_H
#define	CLZ2_H
#define clz(x) clz2(x,0)
#include <stdint.h>

static inline __attribute((always_inline))
unsigned clz2(uint32_t x, int c);

#endif
