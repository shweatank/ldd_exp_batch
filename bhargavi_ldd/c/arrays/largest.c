#include <stdio.h>

int main() {
    int arr[5];
    printf("Enter 5 elements:\n");

    for(int i = 0; i < 5; i++) {
        scanf("%d", &arr[i]);
    }

    int max = arr[0];   // Initialize max with first element
    for(int i = 0; i < 5; i++) {
        if(arr[i] > max) {
            max = arr[i];
        }
    }

    printf("The largest element in the array is %d\n", max);

    return 0;
}

