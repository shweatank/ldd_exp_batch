#include <stdio.h>

int main() {
    unsigned int n, rev = 0;
    printf("Enter number: ");
    scanf("%u", &n);

    for (int i = 0; i < 32; i++) {
        rev = (rev << 1) | (n & 1);
        n >>= 1;
    }

    printf("Reversed bits value = %u\n", rev);
    return 0;
}

