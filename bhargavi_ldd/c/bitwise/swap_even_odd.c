#include<stdio.h>
int swap_evenodd(uint32_t num)
{
	return (num & 0xAAAAAAAA)<<1 |(num & 0x55555555)>>1;
}
int main()
{
	uint32_t num=23;
	printf("after swapping is %d\n", swap_evenodd(num));
}

