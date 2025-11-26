#include <stdio.h>

int main(){
	int num = 0;
	
	printf("Enter Num: ");
	scanf("%d",&num);
	
	int num_l = (num << 1);
	int num_r = (num >> 1);
	//printf("left shift : %d right shift : %d\n",num_l, num_r);

	num_l &= 0xAAAAAAAA;
	num_r &= 0x55555555;
	//printf("left shift : %d right shift : %d\n",num_l, num_r);
	int swapbit = num_l | num_r;

	printf("swapbits : %d\n",swapbit);
}
