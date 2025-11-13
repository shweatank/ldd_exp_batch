#include<stdio.h>
int main()
{
int s;
scanf("%d",&s);
int a[s],even=0,odd=0;

for(int i=0;i<s;i++)
{
    scanf("%d",&a[i]);
    if(a[i]&0x1) 
        odd++;
    else
        even++;
}
printf("even:--%d odd:--%d\n",even,odd);
return 0;
}

