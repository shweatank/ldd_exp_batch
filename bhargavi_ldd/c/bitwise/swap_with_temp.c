#include<stdio.h>
int main()
{
	int a=10,b=20;
	printf("before swapping the result is %d %d\n",a,b);
	int temp=a^b;
	b=temp^b;
	a=temp^a;
	printf("after swapping the result is %d %d\n",a,b);
}
