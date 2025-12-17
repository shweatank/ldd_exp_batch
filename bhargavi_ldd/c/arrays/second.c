#include <stdio.h>
#include <stdlib.h>

int main()
{
    int arr[5];
    int n = sizeof(arr)/sizeof(arr[0]);

    printf("Enter 5 elements in an array:\n");
    for(int i=0; i<n; i++)
    {
        scanf("%d", &arr[i]);
    }

    // Initialize largest and second largest
    int max1 = arr[0];
    int max2 = -2147483648; // Smallest possible int

    for(int i=1; i<n; i++)
    {
        if(arr[i] > max1)
        {
            max2 = max1;   // update second largest
            max1 = arr[i]; // update largest
        }
        else if(arr[i] > max2 && arr[i] < max1)
        {
            max2 = arr[i]; // update second largest only
        }
    }

    if(max2 == -2147483648)
        printf("No second largest element (all elements equal)\n");
    else
        printf("The second largest element in the array is %d\n", max2);

    return 0;
}

