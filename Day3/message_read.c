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
msgrcv(id,&message,sizeof(message),0,0);
printf("\nQueue:%d\n",id);
printf("Got Message:--%s\n",message.t);
msgctl(id,IPC_RMID,NULL);
return 0;
}
