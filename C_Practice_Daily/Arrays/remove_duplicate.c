#include<stdio.h>
int main()
{
int s;
scanf("%d",&s);
int a[s];
for(int i=0;i<s;i++)
scanf("%d",&a[i]);

for(int i=0;i<s;i++)
{
    for(int j=i+1;j<s;j++)
    {
        if(a[i]==a[j])
        {
            for(int k=j;k<s-1;k++)
            {
                a[k]=a[k+1];
            }
           s--;
           j--; 
        }
    }
}
    for(int i=0;i<s;i++)
    printf("%d ",a[i]);
    return 0;
}
