#include<stdio.h>
unsigned int extractbits(int num,int start,int end)
{
	int numbits=end-start+1;
	int mask=(1<<numbits)-1;
	return (num>>start)&mask;
}
int main()
{
unsigned int num=0b000111001000;
	int start=2,end=5;
	printf("extracted bits is %u\n",extractbits(num,start,end));
	}

