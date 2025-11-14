#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>

struct msgbuffer{
   long mtype;
   char mtext[30];
}message;

int main()
{
//  int msgid = 32764;

    key_t key = ftok("./myfifo", 65);
    int msgid = msgget(key, 0666 | IPC_CREAT);
  message.mtype = 1;
//  strcpy(message.mtext, "opensource.com");

  msgrcv(msgid,&message,sizeof(message.mtext),0,0);
  printf("message: %s\n",message.mtext);
  printf("Queue : %d \n",msgid);
  msgctl(msgid,IPC_RMID,NULL);
  return 0;
}

