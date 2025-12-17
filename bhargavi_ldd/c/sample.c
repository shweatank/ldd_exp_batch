#include <stdio.h>
int add(int a,int b)
{
    int c=a+b;
    return c;
}
int main()
{
    int a=2,b=3;
    int c = add(a, b);
    printf("sum = %d\n", c);
    return 0;
}
