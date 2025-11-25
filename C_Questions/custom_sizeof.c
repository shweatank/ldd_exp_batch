#include <stdio.h>

#define SIZE_OF(a)((char *)(ptr+1) - (char *)(ptr))

int main(){
	
	float num = 2;
	float *ptr = &num;

	int size = SIZE_OF(ptr);
	printf("The size of float is : %d\n",size);




}
