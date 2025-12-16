#include <stdio.h>


int g = 5;
static int p=10;
void main() {
    static int s = 1;  

    printf("g = %d, s = %d\n,p=%d\n", g, s,p);
}
 

