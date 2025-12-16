#include <stdio.h>
int num1 = 4;
static int num2 = 5;
void main()
{
    printf("%p = %d\n  %p = %d\n",&num1,num1,&num2,num2);
}
