#include <stdio.h>

// In C, structure members must be instance-specific; 
// each instance of the structure needs its own separate copy of the member. 
// Static variables, however, are not instance-specific—they are shared across all instances of a function, file, or class (depending on the scope). 
// This is why you cannot declare a static variable as a member of a structure.

struct node {
    static int a; 
}


int main()
{
    return 0;
}
