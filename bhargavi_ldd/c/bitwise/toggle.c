#include<stdio.h>
int toggleithbit(int n,int i)
{
	return (n^(1<<(i-1)));
}
int main()
{
	int n=13,i=2;
	int result=toggleithbit(n,i);
	printf("number after toggle ith bit %d is %d\n",i,result);

}

