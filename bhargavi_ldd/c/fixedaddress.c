#include <stdio.h>
#include <stdlib.h>
int main() {
	unsigned int* p_fixed_address = (unsigned int*)0x12345678;
	*p_fixed_address = 100;	
	if (ptr == NULL) 
	{
		printf("Memory allocation failed\n");
		return 1;
	}
*ptr = 10;
    printf("Value = %d\n", *ptr);
    free(ptr);
    return 0;
}
