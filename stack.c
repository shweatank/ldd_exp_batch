#include <stdio.h>

void fun() {
    int x = 10;   // stored in stack
    printf("Stack variable x = %d\n", x);
}

int main() {
    fun();
    return 0;
}

