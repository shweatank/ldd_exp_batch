#include<stdio.h>
#include<limits.h>
int main()
{
int s;
scanf("%d",&s);
int a[s];
for(int i=0;i<s;i++)
scanf("%d",&a[i]);

int max1=INT_MIN,max2=INT_MIN;

for(int i=0;i<s;i++)
{
    if(a[i]>max1)
    {
        max2=max1;
        max1=a[i];
    }
    if(a[i]<max1 && a[i]>max2)
    max2=a[i];
}
printf("second largetst number:--%d\n",max2);
return 0;
}
