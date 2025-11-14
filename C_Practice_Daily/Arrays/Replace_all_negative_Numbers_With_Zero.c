#include<stdio.h>
int main()
{
int s;
scanf("%d",&s);
int a[s];

for(int i=0;i<s;i++)
{
    scanf("%d",&a[i]);
    if(a[i]<0)
        a[i]=0;
}

for(int i=0;i<s;i++)
printf("%d ",a[i]);

return 0;
}

