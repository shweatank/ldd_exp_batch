#include<stdio.h>
#include<math.h>
int main()
{
	int n;
	printf("enter the number\n");
	scanf("%d",&n);
	if(n > 0 && ((n & (n-1)) == 0) && (n & 0x55555555))
	{
		printf("the number is the power of 4\n");
	}
	else
	{
		printf("the number is not the power of 4\n");
	}
}
