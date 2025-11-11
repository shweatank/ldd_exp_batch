#include <stdio.h>

int add(int a, int b)
{
    return a + b;
}

int sub(int a, int b)
{
    return a - b;
}

int mul(int a, int b)
{
    return a * b;
}

int div(int a, int b)
{
    return a / b;
}

int main()
{
    int (*fp[])(int, int) = {&add, &sub, &mul, &div};

    printf("%d\n",fp[0](20,10));
    printf("%d\n",fp[1](20,10));
    printf("%d\n",fp[2](20,10));
    printf("%d\n",fp[3](20,10));

    return 0;
}
