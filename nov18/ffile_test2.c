#include <stdio.h>
void main()
{
   int var = 4357;
   char d[30];
   int l = snprintf(d,sizeof(d),"%d",var);
   write(1,d,l);

   char buff[5]= "hello";
   write(1,buff,sizeof(buff));

   
}
