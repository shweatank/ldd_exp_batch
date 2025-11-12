#include<stdio.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include <string.h>

struct msgBuffer
{
    char text[24];
} msg;

int main()
{
    int msgid = 32764;
    msgrcv(msgid, &msg, sizeof(msg), 0, 0);
    printf("Queue: %d\n",msgid);
    printf("Got the message: %s\n", msg.text);
    msgctl(msgid, IPC_RMID,NULL);
    
    return 0;
}