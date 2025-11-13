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
    int t=0;
    for(int j=0;j<i;j++)
    {
        if(a[i]==a[j])
        {
            t++;
            break;
        }
    }
    if(t==0)
    {
        for(int j=i+1;j<s;j++)
        {
            if(a[i]==a[j])
                t++;
        }
        printf("value:--%d count:--%d\n",a[i],t+1);
    }
}
    return 0;
}
