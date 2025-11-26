#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
int main()
{
int fd=open("sai1.txt",O_CREAT|O_RDWR,0777);

int a=1234;
char d[100];

int l=snprintf(d,sizeof(d),"%d",a);
write(1,d,l);
return 0;
}
