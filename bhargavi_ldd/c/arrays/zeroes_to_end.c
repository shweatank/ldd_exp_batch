#include <stdio.h>

int main()
{
    int arr[10];
    int n = sizeof(arr)/sizeof(arr[0]);

    printf("Enter %d elements:\n", n);
    for(int i=0; i<n; i++)
    {
        scanf("%d", &arr[i]);
    }

    int count = 0; // index for non-zero elements

    // Traverse the array
    for(int i=0; i<n; i++)
    {
        if(arr[i] != 0)
        {
            arr[count++] = arr[i]; // move non-zero forward
        }
    }

    // Fill remaining positions with 0
    while(count < n)
    {
        arr[count++] = 0;
    }

    printf("Array after moving zeroes to the end:\n");
    for(int i=0; i<n; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}
	

