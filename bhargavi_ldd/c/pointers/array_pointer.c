#include<stdio.h>
int main()
{
int arr[]={1,2,3,4,5};
//int arr[100];
int *ptr;
ptr=arr;

/*for(int i=0;i<100;i++)
{
arr[i]=i+1;
}*/

for(int i=0;i<100;i++)
{
printf("%d\n",arr[i]);
}
}
