#include <stdio.h>
int add(int,int);
void main()
{
   int a=10, b=20;
   printf("%d %d \n",a,b);

   printf("%d\n",add(a,b));
   
}

int add(int a,int b)
{
   int c;
   c= a+b;
  return c;
}
