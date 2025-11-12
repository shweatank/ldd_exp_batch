#include<stdio.h>
#include<sys/msg.h>
#include<stdio.h>
#include<string.h>
struct stu{
    char t[24];
}message;

int main()
{
    int id=1234;
    strcpy(message.t,"opensource.com");
    msgsnd(id,&message,sizeof(message),0);
    printf("Message:%s\n",message.t);
    printf("Queue:%d\n",id);
    return 0;
}


