#include<stdio.h>
int main()
{
	int n=-12;
	if(((n>>31)&1)==1)
	{
		printf("%d is negative\n",n);
	}
	else
	{
		printf("%d is positive\n",n);
	}
}

