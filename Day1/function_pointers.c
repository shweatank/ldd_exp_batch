#include<stdio.h>
typedef struct stu{
int (*fp)(int ,int );
int (*sp)(int ,int );
int (*mp)(int ,int );
int (*dv)(int ,int );
}var;

typedef union uni{
var w;
int (*ar[4])(int,int);
}sai;
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

int main()
{
int a,b;
scanf("%d%d",&a,&b);
sai q;
q.ar[0]=add;
printf("union add:--%d\n",q.ar[0](a,b));
q.ar[1]=sub;
printf("union sub;--%d\n",q.ar[1](a,b));
q.ar[2]=mul;
printf("union mul:--%d\n",q.ar[2](a,b));
q.ar[3]=dev;
printf("union dev;--%d\n",q.ar[3](a,b));
q.w.fp=add;
printf("add:--%d\n",q.w.fp(a,b));
q.w.sp=sub;
printf("sub:--%d\n",q.w.sp(a,b));
q.w.mp=mul;
printf("mul;--%d\n",q.w.mp(a,b));
q.w.dv=dev;
printf("dev:--%d\n",q.w.dv(a,b));
return 0;
}

