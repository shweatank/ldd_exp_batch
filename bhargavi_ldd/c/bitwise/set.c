#include<stdio.h>
int  setithbit(int n,int i)
{
	return n|(1<<(i-1));
}
int main()
{
	int n=13,i=4;
	int result=setithbit(n,i);
	printf("number after setting the ith bit %d is %d\n",i,result);
}

