#include<stdio.h>
int main()
{
	int arr[10];
	int n=sizeof(arr)/sizeof(arr[0]);
	printf("enter the array elements\n");
	for(int i=0;i<n;i++)
	{
	 scanf("%d",&arr[i]);
	}
	int max=arr[0];
	for(int i=0;i<n;i++)
	{
		if(arr[i]>max)
		{
			max=arr[i];
		}
	}
	printf("largest element in an array is %d\n",max);
}

