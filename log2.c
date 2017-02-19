#define LOG2(X) ((unsigned) \
   (8 * sizeof (unsigned long long) -\
    __builtin_clzll((X)) - 1))

int main()
{
    printf("%u %u\n", LOG2(0x10), LOG2(0.5));
}
