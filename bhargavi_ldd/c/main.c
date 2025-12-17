// main.c
#include <stdio.h>
#include "add.h"

int main() {
    int x = 10, y = 20;

    int result = add(x, y);

    printf("Addition = %d\n", result);

    return 0;
}

