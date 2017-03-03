#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <assert.h>
#include <omp.h>

#if defined(recursive)
#define clz(x) clz2(x,0)
#endif

#if defined(overload)
#include "clz.hpp"
#else
#include "clz.h"
#endif

static inline __attribute__((always_inline))
void get_cycles(unsigned *high, unsigned *low)
{
    asm volatile ("CPUID\n\t"
                  "RDTSC\n\t"
                  "mov %%edx, %0\n\t"
                  "movl %%eax, %1\n\t": "=r" (*high), "=r" (*low)::"%rax","%rbx","%rcx","%rdx"
                 );
}

static inline __attribute__((always_inline))
void get_cycles_end(unsigned *high, unsigned *low)
{
    asm volatile("RDTSCP\n\t"
                 "mov %%edx, %0\n\t"
                 "mov %%eax, %1\n\t"
                 "CPUID\n\t": "=r" (*high), "=r" (*low)::"%rax","%rbx","%rcx","%rdx"
                );
}

static inline __attribute__((always_inline))
uint64_t diff_in_cycles(unsigned high1, unsigned low1,
                        unsigned high2, unsigned low2)
{
    uint64_t start,end;
    start = (((uint64_t) high1 << 32) | low1);
    end = (((uint64_t) high2 << 32) | low2);
    return end - start;
}

static inline __attribute__((unused))
double diff_in_second(struct timespec t1, struct timespec t2)
{
    struct timespec diff;
    if (t2.tv_nsec-t1.tv_nsec < 0) {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec - 1;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec + 1000000000;
    } else {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec;
    }
    return (diff.tv_sec + diff.tv_nsec / 1000000000.0);
}

int main(int argc, char *argv[])
{
    FILE *output = NULL;
    uint64_t timec, time_all = 0;
    unsigned timec_high1, timec_low1, timec_high2, timec_low2;

#if defined(correct)

    for (int try = 0; try < 20; try++) {
                    timec = 0;
                    get_cycles(&timec_high1, &timec_low1);
                    printf("%u:%d \n", 0, clz(0));
                    assert((sizeof(uint32_t) * 8) == clz(0));
                    for (uint32_t i = 0; i < 31; i++) {
                        printf("%u:%d \n", 1 << i, clz(1 << i));
                        for (uint32_t j = (1 << i); j < (1 << (i + 1)); j++) {
                            assert( __builtin_clz (j) == clz(j));
                        }
                    }
                    printf("%u:%d \n", 1u << 31, clz(1u << 31));
                    for (uint32_t j = (1u << 31); j < UINT32_MAX; j++)
                        assert(__builtin_clz(j) == clz(j));
                    assert(__builtin_clz(UINT32_MAX) == clz(UINT32_MAX));
                    get_cycles_end(&timec_high2, &timec_low2);
                    timec = diff_in_cycles(timec_high1, timec_low1, timec_high2, timec_low2);
                    printf("executiom time : %lu cycles\n", timec);
                }

#else
    assert(argv[1] && argv[2] && "insert argument");
    unsigned int min = atoi(argv[1]);
    unsigned int max = atoi(argv[2]);

#if defined(recursive)
    output = fopen("recursive.txt","a");
#elif defined(iteration)
    output = fopen("iteration.txt","a");
#elif defined(byte)
    output = fopen("byte.txt","a");
#elif defined(binary)
    output = fopen("binary.txt","a");
#elif defined(harley)
    output = fopen("harley.txt","a");
#elif defined(overload)
    output = fopen("overload.txt","a");
#endif
    uint64_t timecall;
    for (uint32_t i = min; i < max; i++) {
        timecall = 0;
#ifdef MP
        #pragma omp parallel for
#endif
        for (uint32_t j = 0; j < 100; j++) {
            get_cycles(&timec_high1, &timec_low1);
            clz(i);
            get_cycles_end(&timec_high2, &timec_low2);
            timec = diff_in_cycles(timec_high1, timec_low1, timec_high2, timec_low2);
            timecall += timec;
        }
        time_all += (timecall / 100);
        fprintf(output, "%d %lu cycles\n", i, timecall / 100);
        printf("%d %lu cycles\n", i, timecall / 100);
    }

    fclose(output);

    printf("took %lf million cycles\n", time_all / 1000000.0);
#endif

    return 0;
}
