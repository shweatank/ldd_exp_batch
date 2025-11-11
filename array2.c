#include<stdio.h>
#include<limits.h>
int main()
{
	int arr[5];
	int n=sizeof(arr)/sizeof(arr[0]);
	printf("enter the array elements\n");
	for(int i=0;i<n;i++)
	{
		scanf("%d",&arr[i]);
	}
	int first=INT_MIN,second=INT_MIN;
	for(int i=0;i<n;i++)
	{
		if(arr[i]>first)
		{
			second=first;
			first=arr[i];
		}
		else if(arr[i]>second && arr[i]!=first)
		{
			second=arr[i];
		}
	}
	if(second==INT_MIN)
	{
		printf("no second element exis\n");
	}
	else
	{
		printf("the second largest element in an array is %d\n",second);
	}
}
