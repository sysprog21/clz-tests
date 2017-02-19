#include "clz.hpp"
#include <iostream>
#include <cstdint>
using namespace std;
static const int magic[]={2,1,0,0};

//static inline __attribute((always_inline))
unsigned clz(uint32_t x)
{
    uint16_t upper = uint16_t(x >> 16);
    uint16_t lower = uint16_t(x & 0xFFFF);
    return upper ? clz(upper) : 16 + clz(lower);
}

unsigned clz(uint16_t x)
{
    uint8_t upper = uint8_t(x >> 8);
    uint8_t lower = uint8_t(x & 0xFF);
    return upper ? clz(upper) : 8 + clz(lower);
}

unsigned clz(uint8_t x)
{
    uint8_t upper = uint8_t(x >> 4);
    uint8_t lower = uint8_t(x & 0xF);
    return upper ? magic[upper] : 8 + magic[lower];
}
