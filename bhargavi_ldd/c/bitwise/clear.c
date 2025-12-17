#include<stdio.h>
int clearithbit(int n,int i)
{
	return (n & ~(1<<(i-1)));
}
int main()
{
	int n=13,i=3;
	int result=clearithbit(n,i);
	printf("number after clearing the bit %d is %d\n",i,result);
}
