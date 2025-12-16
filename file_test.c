#include <stdio.h>
#include <string.h>

int  main()
{
    FILE *fp;
    int send_len = 0,recv_len = 0;

    char send_buff[20] = "hellp world\n";
    char recv_buff[20];

   fp = fopen("test.txt","w+");
   if(fp < 0)
    {
       printf("error opening a file\n");
       return 0;
     }
 printf("file opened sucessfully\n");
 
  send_len = fwrite(send_buff, sizeof(char), strlen(send_buff), fp);
 if(send_len != strlen(send_buff)) 
  {
  perror("fwrite");
        fclose(fp);
        return 0;
  }
 printf("fil write successfully\n");
 
  fseek(fp, 0, SEEK_SET);

  recv_len  = fread(recv_buff, sizeof(char), strlen(recv_buff), fp);
  /* if(recv_len == strlen(send_buff))
    {
    perror("fread");
          fclose(fp);
          return 0;
    }*/
   printf("file read = %s",recv_buff);
   printf("fil read successfully\n");

  
if (fclose(fp) != 0) {
        perror("fclose");
        return 0;
    }

}
   
