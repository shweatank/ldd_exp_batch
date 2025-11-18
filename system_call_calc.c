#include <stdio.h>

struct calc{
    int my_add;
    int my_sub;
    int my_mul;
    int my_div;

} calc2;

int my_add(int a, int b)
{
    return a +b;
}

int my_sub(int a, int b)
{
   return a - b;
}

int my_mul(int a, int b)
{
    return a * b;
}

int my_div(int a, int b)
{
   return a / b;
}

int main()
{

    int (*add)(int a, int b); 
    int (*sub)(int a, int b); 
    int (*mult)(int a, int b); 
    int (*div)(int a, int b); 

    calc2.my_add = my_add(5,3);
    calc2.my_sub = my_sub(5,3);
    calc2.my_mul = my_mul(5,3);
    calc2.my_div = my_div(5,3);

    return 0;
}
