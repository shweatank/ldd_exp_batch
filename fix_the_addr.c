#include <stdio.h>

#define ADDRESS 0xAFFE341C

int main() {

    int a = 10;
    int *p = (int *)ADDRESS;
    printf("%p\n", p);

    // int *p = &a;
    // printf("%p\n", p);

    return 0;
}