#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>


int add(int a,int b)
{
return a+b;
}

int sub(int a,int b)
{
    return a-b;
}

int mul(int a,int b)
{
    return a*b;
}

int dev(int a,int b)
{
    return a/b;
}

struct stu{
    int(*ad)(int ,int );
    int(*su)(int ,int);
    int(*mu)(int ,int );
    int(*de)(int , int);
};

int main()
{
    int a,b;
    scanf("%d%d",&a,&b);
    struct stu x={add,sub,mul,dev};
    printf("addition:--%d\n",x.ad(a,b));
    printf("subtraction:--%d\n",x.su(a,b));
    printf("multiplicaton:--%d\n",x.mu(a,b));
    printf("devision:--%d\n",x.de(a,b));
    return 0;
} 
