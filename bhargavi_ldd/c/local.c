#include <stdio.h>

int main()
{
    const int x = 5;

    int *p = (int *)&x;
    *p = 20;

    printf("x = %d\n", x); 
    printf("*p = %d\n", *p);

    return 0;
}


