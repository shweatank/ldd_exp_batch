#include<stdio.h>

int main()
{
    const int a = 5;
    int *p = &a;

    printf("%d\n", a);
    *p = 20;
    printf("%d\n", a);
     
    // a = 10;

    return 0;
}
