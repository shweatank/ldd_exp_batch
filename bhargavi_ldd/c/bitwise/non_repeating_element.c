#include<stdio.h>
int non_repeating(int arr[],int n)
{
	int res=0;
	for(int i=0;i<n;i++)
	{
		res^=arr[i];
	}
	return res;
}
int main()
{
	int arr[]={1,2,2,3,3,1,5};
	int n=sizeof(arr)/sizeof(arr[0]);
	int res=non_repeating(arr,n);
	printf("the non reparting element in an array is %d\n",res);
}



