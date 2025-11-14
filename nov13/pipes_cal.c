#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int fd[2];
    int arr[3];

   pipe(fd);
   
   if(fork() == 0)
   {
      close(fd[1]); //close write end
      printf("before read child\n");
      read(fd[0], arr, sizeof(arr));
      switch(arr[0])
      {
           case 1: printf(" add = %d\n",arr[1]+arr[2]);
                   break;
           case 2: printf(" sub = %d\n",arr[1]-arr[2]);
                   break;
           case 3: printf( "mul = %d\n",arr[1]*arr[2]);
                   break;
           default :printf("entered invali choice\n");
                   break;
        } 
       printf("after read child\n");
      close(fd[0]);
     }
   else 
  {
      close(fd[0]); //close read end
      printf(" cal 1.add 2.sub 3.mul\n");
      printf("enter choice\n");
      scanf("%d",&arr[0]);
      printf("enter elements\n");
      scanf("%d %d",&arr[1],&arr[2]);
      printf("before write child\n");
      write(fd[1], arr, sizeof(arr));
      printf("parent \n");
//      wait(NULL);
      close(fd[1]);
    }

return 0;
}
