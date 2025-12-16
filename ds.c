#include <stdio.h>

int g = 5;  // global variable (stored in global memory)

void fun() {
    static int s = 1;  // static memory
    s++;
    printf("Static s = %d\n", s);
}

int main() {
    printf("Global g = %d\n", g);
    fun();
    fun();
    return 0;
}

