#include<stdio.h>
#include<math.h>
int main()
{
	int n;
	printf("enter the number\n");
	scanf("%d",&n);
	if(n>0 && (n & (n - 1)) == 0)
	{
		printf("the number is a power of 2\n");
	}
	else
	{
		printf("the number is not a power of 2\n");
	}
}
