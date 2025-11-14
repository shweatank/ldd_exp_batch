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
while()
{  
  strcpy(message.mtext, "opensource");
  msgsnd(msgid,&message,sizeof(message.mtext),0);

  

}
  //printf("message: %s\n",message.mtext);
  //printf("Queue : %d \n",msgid);
  return 0;
}

