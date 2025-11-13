#include<stdio.h>
int main()
{
int s;
scanf("%d",&s);
int a[s];
for(int i=0;i<s;i++)
scanf("%d",&a[i]);

for(int i=0,j=s-1;i<j;i++,j--)
{
    a[i]^=a[j];
    a[j]^=a[i];
    a[i]^=a[j];
}

for(int i=0;i<s;i++)
    printf("%d ",a[i]);
}
