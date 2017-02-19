#ifndef CLZ_H
#define	CLZ_H

#include <stdint.h>

#if defined(recursive) 
#define clz(x) clz2(x,0)
static const int mask[]={0,8,12,14};
static const int magic[]={2,1,0,0};

//static inline __attribute((always_inline))
unsigned clz2(uint32_t x,int c)
{
    if (!x && !c) return 32;

    uint32_t upper = (x >> (16 >> c));
    uint32_t lower = (x & (0xFFFF>>mask[c]));
    if (c == 3) return upper ? magic[upper] : 2 + magic[lower];
    return upper ? clz2(upper, c + 1) : (16 >> (c)) + clz2(lower, c + 1);
}

#elif defined(iteration) 

static inline __attribute((always_inline))
unsigned clz(uint32_t x)
{
    int n = 32, c = 16;
    do {
        uint32_t y = x >> c;
        if (y) { n -= c; x = y; }
        c >>= 1;
    } while (c);
    return (n - x);
}

#elif defined(byte) 

static inline __attribute((always_inline))
unsigned clz(uint32_t x)
{
    if (x == 0) return 32;
    int n = 1;
    if ((x >> 16) == 0) { n += 16; x <<= 16; }
    if ((x >> 24) == 0) { n += 8; x <<= 8; }
    if ((x >> 28) == 0) { n += 4; x <<= 4; }
    if ((x >> 30) == 0) { n += 2; x <<= 2; }
    n = n - (x >> 31);
    return n;
}

#elif defined(binary) 

static inline __attribute((always_inline))
unsigned clz(uint32_t x)
{
    if (x == 0) return 32;
    int n = 0;
    if (x <= 0x0000FFFF) { n += 16; x <<= 16; }
    if (x <= 0x00FFFFFF) { n += 8; x <<= 8; }
    if (x <= 0x0FFFFFFF) { n += 4; x <<= 4; }
    if (x <= 0x3FFFFFFF) { n += 2; x <<= 2; }
    if (x <= 0x7FFFFFFF) { n += 1; x <<= 1; }
    return n;
}

#elif defined(harley) 

static inline __attribute((always_inline))
unsigned clz(uint32_t x)
{
    // CTZ table
#ifdef CTZ
    static uint8_t const Table[] = {
        0xFF,    0, 0xFF,   15, 0xFF,    1,   28, 0xFF,
          16, 0xFF, 0xFF, 0xFF,    2,   21,   29, 0xFF,
        0xFF, 0xFF,   19,   17,   10, 0xFF,   12, 0xFF,
        0xFF,    3, 0xFF,    6, 0xFF,   22,   30, 0xFF,
          14, 0xFF,   27, 0xFF, 0xFF, 0xFF,   20, 0xFF,
          18,    9,   11, 0xFF,    5, 0xFF, 0xFF,   13,
          26, 0xFF, 0xFF,    8, 0xFF,    4, 0xFF,   25,
        0xFF,    7,   24, 0xFF,   23, 0xFF,   31, 0xFF,
    };

    // CLZ table
#else
    static uint8_t const Table[] = {
        32, 31,  0, 16,  0, 30,  3, 0, 15,  0,  0,  0, 29, 10, 2,  0,
         0,  0, 12, 14, 21,  0, 19, 0,  0, 28,  0, 25,  0,  9, 1,  0,
        17,  0,  4,  0,  0,  0, 11, 0, 13, 22, 20,  0, 26,  0, 0, 18,
         5,  0,  0, 23,  0, 27,  0, 6,  0, 24,  7,  0,  8,  0, 0,  0
    };
#endif
    /* Propagate leftmost 1-bit to the right */
    x = x | (x >> 1);
    x = x | (x >> 2);
    x = x | (x >> 4);
    x = x | (x >> 8);
    x = x | (x >> 16);
 
    /* x = x * 0x6EB14F9 */
    x = (x << 3) - x;   /* Multiply by 7. */
    x = (x << 8) - x;   /* Multiply by 255. */
    x = (x << 8) - x;   /* Again. */
    x = (x << 8) - x;   /* Again. */

    return Table[(x >> 26)];
}
#endif

#endif
