#include<stdio.h>
int main()
{
	int arr[5]={1,2,3,5};
	int n=sizeof(arr)/sizeof(arr[0]);
	int actual_sum=(n)*(n+1)/2;
	int sum=0;
	for(int i=0;i<5;i++)
	{
		sum+=arr[i];
	}
	int missing=actual_sum-sum;
	printf("the missing number is %d\n",missing);
}

