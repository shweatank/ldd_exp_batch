#include <stdio.h>
#include <signal.h>
//void (*sighandler_t)(int);
void print_num(int num)
{
  printf("signal custom %d\n",num);
}
void main()
{
     while(1)
   {
     
     signal(SIGTSTP, print_num);
     }
   
}
