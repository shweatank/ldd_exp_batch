#include <stdio.h>

void counter() {
    static int x = 0;   // value saved between calls
    x++;
    printf("x = %d\n", x);
}

int main() {
    counter();  // x = 1
    counter();  // x = 2
    counter();  // x = 3
    return 0;
}

