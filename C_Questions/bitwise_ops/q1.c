//WAP to swap bits(4 bits) of a number
//input  = 1011 0111
//output = 0111 1011

#include <stdio.h>

int main(){
	int a = 171;

	printf("number before swapping: %d\n",a);
	int swapbits = ((a & 0x0F) << 4 | (a & 0xF0) >> 4);
	printf("number after swapping: %d\n",swapbits);
}
