#include <stdio.h>

int main(void)
{
    size_t n = 1000;
    size_t ret = 0;
    while (n >> 6) {
        ret += (n & 1);
        n >>= 1;
    }

    printf("%ld\n", ret + n);
    printf("%d\n", __builtin_clz(1000));
}