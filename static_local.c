#include <stdio.h>

static int globalStatic = 5;

int main()
{
    static int localStatic = 10;
    return 0;
}