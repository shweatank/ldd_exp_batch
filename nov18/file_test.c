#include <stdio.h>
#include <string.h>

void main()
{
    FILE *fp;
    int send_len = 0;
    char send_buff[20] = "hellp world\n";
    char read_buff[20];

   fp = fopen("test.txt","w+");
   if(fp < 0)
    {
       printf("error opening a file\n");
       return 0;
     }
 printf("file opened sucessfully\n");
 
 send_len  = fwrite(send_buff, sizeof(char), strlen(send_buff), fp);
 if(send_len == strlen(send_buff)) 
  {
  perror("fwrite");
        fclose(fp);
        return 0;
  }
 printf("fil write successfully\n");
 
  fseek(fp, 0, SEEK_SET);
  
if (fclose(fp) != 0) {
        perror("fclose");
        return 0;
    }

}
/*fp = fopen("test.txt",o_WRONLY,0666);
     if(fp < 0)
      {
         printf("error opening a file\n");
         return 0;
       }
   printf("file opened sucessfully\n");
*/
   
