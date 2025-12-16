#include <stdio.h>
#include <stdlib.h>

int main() {
    int *p = (int*)malloc(sizeof(int)); // allocated in heap
    *p = 20;

    printf("Heap value = %d\n", *p);

    free(p); // freeing memory
    return 0;
}

