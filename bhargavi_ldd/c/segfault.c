#include <stdio.h>

int main() {
    int *ptr = NULL;   // pointer points to nothing
    printf("Value = %d\n", *ptr);  // ❌ dereferencing NULL -> crash
    return 0;
}

