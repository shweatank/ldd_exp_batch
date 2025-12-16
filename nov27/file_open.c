#include <stdio.h>
void main()
{
     char buff[5];
     int fd;
    fd = fopen("file.txt","w");
    if(fd < 0)
      {
          printf("error opening file\n");
           return 0;
        }

     printf("enter data\n");
     scanf("%s",buff);
     fwrite(fd,buff,sizeof(buff));
    
 
}
