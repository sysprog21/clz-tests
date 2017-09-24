#include "clz.h"

static inline __attribute((always_inline))
unsigned clz(uint32_t x)
{
#ifdef CTZ
    static uint8_t const Table[] = {
        0xFF, 0, 0xFF, 15, 0xFF, 1, 28, 0xFF,
        16, 0xFF, 0xFF, 0xFF, 2, 21, 29, 0xFF,
        0xFF, 0xFF, 19, 17, 10, 0xFF, 12, 0xFF,
        0xFF, 3, 0xFF, 6, 0xFF, 22, 30, 0xFF,
        14, 0xFF, 27, 0xFF, 0xFF, 0xFF, 20, 0xFF,
        18, 9, 11, 0xFF, 5, 0xFF, 0xFF, 13,
        26, 0xFF, 0xFF, 8, 0xFF, 4, 0xFF, 25,
        0xFF, 7, 24, 0xFF, 23, 0xFF, 31, 0xFF,
    };

#else
    static uint8_t const Table[] = {
        32,31, 0,16, 0,30, 3, 0,15, 0, 0, 0,29,10, 2, 0,
        0, 0,12,14,21, 0,19, 0, 0,28, 0,25, 0, 9, 1, 0,
        17, 0, 4, 0, 0, 0,11, 0,13,22,20, 0,26, 0, 0,18,
        5, 0, 0,23, 0,27, 0, 6,0,24, 7, 0, 8, 0, 0, 0
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
