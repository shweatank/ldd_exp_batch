#include<stdio.h>
int main()
{
int a,b;
scanf("%d%d",&a,&b);
int c=0;
while(b)
{
    if(b&0x1)
        c+=a;
    a<<=1;
    b>>=1;
}
printf("result:--%d\n",c);
return 0;
}
