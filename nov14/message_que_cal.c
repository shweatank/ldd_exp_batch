#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

struct msgbuffer {
	long type;
	int text[24];
}message;

int flag_parent = 0;
int print_flag= 0;
int child_send_flag = 0;

void signal_flag(int a)
{
    //flag_parent = 1;
    if(child_send_flag == 1)
       {
        print_flag =1;
        child_send_flag = 0;
        }
     else
       flag_parent = 1;
}
int main() {
      // struct msgbuffer message;
       key_t key;
       int msgid;
       key = ftok("file.c", 65);
       msgid = msgget(key, 0666 | IPC_CREAT);

	pid_t pid = fork();

	if(pid < 0) {
		perror("fork");
		return -1;
	}
	else if(pid == 0) {
		struct msgbuffer message;
                int res = 0;
	/*	key_t key;
		int msgid;
                int res = 0;
		printf("entered into the child\n");
		// ftok to generate unique key
		key = ftok("file.c", 65);
		// msgget creates a message queue
		// and returns identifier
		msgid = msgget(10, 0666);
*/
		message.type=1;
		// msgrcv to receive message
                printf("before child\n");
               // sleep(5);
                printf("before child recv %d\n",getpid());
		int ret = msgrcv(msgid, &message,sizeof(message.text),message.type, 0);
                printf("%d\n",ret);
                printf("after child\n");
		printf("child recv ret = %d\n", ret);
		int a,b, op;
		a = message.text[1];
		b = message.text[2];
		op = message.text[0];
		switch(op) {
			case 1:
				//printf("Addition %d and %d is %d\n", a, b, a+b);
                                res = a+b;
				break;
			case 2:
				printf("Subtraction %d and %d is %d\n", a, b, a-b);
				break;
			case 3:
				printf("Subtraction %d and %d is %d\n", a, b, a*b);
				break;
			case 4:
				printf("Subtraction %d and %d is %d\n", a, b, a/b);
				break;
		}
           
                message.text[3] = res;
                child_send_flag = 1;
                message.type =2;
                int i=msgsnd(msgid, &message, sizeof(message.text), 0);
                printf("child sent Ret = %d\n", i);
               
		// to destroy the message queuehar a,b,c;
		//msgctl(msgid, IPC_RMID, NULL);
		return 0;
	}
	else{
		struct msgbuffer message;
/*		key_t key;
		int msgid;

		// ftok to generate unique key
		key = ftok("file.c", 65);

		// msgget creates a message queue
		// and returns identifier
		msgid = msgget(10, 0666 | IPC_CREAT);
      */
		message.type = 1;
                

		while(1)
               {
                signal(SIGINT,signal_flag);
                 if(flag_parent == 1)
                  {
		int a,b,choice;
		//char c;
                printf("1.add 2.sub 3.dev 4.mul\n");
		printf("Enter choice  a b: \n");
		scanf("%d %d %d", &choice, &a, &b);

		message.text[0] = choice;
		message.text[1] = a;
		message.text[2] = b;
		//message.text[3] = '\0';

		int i=msgsnd(msgid, &message, sizeof(message.text), 0);
		printf("parent sent Ret = %d\n", i);
//		printf("Message: %s\n", message.text);
		printf(" parent -- Queue, %d\n", msgid);
                printf("child id in parent %d %d \n",getpid(),getppid());
                wait(NULL);
		int ret = msgrcv(msgid, &message, sizeof(message.text), 2, 0);
                printf("paren recv ret = %d\n", ret);
                 msgctl(msgid, IPC_RMID, NULL);
                flag_parent = 0;
                  }
                else if(print_flag == 1){
                    printf(" parent result = %d\n",message.text[3]);
                    print_flag = 0;
                        }
                }
              
	}
	return 0;
}
