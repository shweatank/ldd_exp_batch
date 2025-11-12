#include <stdio.h>
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
    strcpy(msg.text, "shared message");
    msgsnd(msgid, &msg, sizeof(msg),0);
    printf("Message: %s\n",msg.text);
    printf("Queue: %d\n",msgid);
    return 0;
}