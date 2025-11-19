#include <stdio.h>
struct calculator 
{
  void (*ADD)(int a,int b);
  void (*SUB)(int a,int b);
  void (*DIV)(int a,int b);
  void (*MUL)(int a,int b);
};

void add_func(int a,int b)
{
   printf("add = %d\n", (a+b));
}
void sub_func(int a,int b)
  {
    printf("sub = %d\n", (a-b));
 }
void div_func(int a,int b)
  {
     printf("div = %d\n", (a/b));
  }
void mul_func(int a,int b)
  {
     printf("mul= %d\n", (a*b));
  }


void main()
{
int choice,a,b;
   struct calculator calc={
                          .ADD = add_func,
                          .SUB = sub_func,
                          .DIV = div_func,
                          .MUL = mul_func,
                            };
   
  while(1){
  printf("1.ADD 2.SUB 3.DIV 4.MUL\n");
   printf("enter choice and a,b\n");
   scanf("%d %d %d",&choice,&a,&b);

 	  switch(1)
  {
         case 1: calc.ADD(a,b);
                 break;
         case 2: calc.SUB(a,b);
                  break;
         case 3: calc.DIV(a,b);
                   break;
        case 4: calc.MUL(a,b);
                   break;
        default : printf("invalid choice \n");
                  break;


     }
 
   }

}
