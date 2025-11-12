#include <stdio.h>

void swap(int *a, int *b)
{
    *a = *a ^ *b;
    *b = *a ^ *b;
    *a = *a ^ *b;
}

void bbl_sort(int a[], int l)
{
    for (int i = 0; i < l - 1; i++)
    {
        for (int j = 0; j < l - i - 1; j++)
        {
            if (a[j] > a[j + 1])
                swap(&a[j], &a[j + 1]);
        }
    }
}

int main()
{
    int a[] = {6, 3, 7, 2};
    int l = sizeof(a) / sizeof(a[0]);

    bbl_sort(a, l);

    for (int i = 0; i < l; i++)
        printf("%d ",a[i]);

    printf("\n");

    return 0;
}