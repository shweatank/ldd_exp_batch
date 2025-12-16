#include <stdio.h>

int main() {
    int a = 5;   // 5 in binary = 0000 0101
    int left, right;

    // Left Shift
    left = a << 4;

    // Right Shift
    right = a >> 1;

    printf("Original a = %d\n", a);
    printf("Left Shift (a << 1) = %d\n", left);
    printf("Right Shift (a >> 1) = %d\n", right);

    return 0;
}

