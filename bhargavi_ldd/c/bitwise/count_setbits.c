#include <stdio.h>

int main() {
    int n, count = 0;
    printf("Enter number: ");
    scanf("%d", &n);

    while (n > 0) {
        n = n & (n - 1);  // clear rightmost set bit
        count++;
    }

    printf("Number of set bits = %d\n", count);
    return 0;
}

