#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
int main()
{
int fd=open("sai.txt",O_CREAT|O_RDWR,0777);
char s[100];

int r=read(0,s,sizeof(s));

write(fd,s,sizeof(s));

close(fd);

return 0;
}

