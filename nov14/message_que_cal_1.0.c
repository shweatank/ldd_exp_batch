#include <sys/ipc.h>
#include <stdlib.h>
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

		message.type=1;
                printf("before child recv %d\n",getpid());
		if((msgrcv(msgid, &message,sizeof(message.text),message.type, 0)==-1));
                  {
                    perror("child msgrcv error");
        //             exit(1);
 
                           }
//		printf("after child recv ret = %d\n", ret);
		int a,b, op;
		a = message.text[1];
		b = message.text[2];
		op = message.text[0];
                printf("test 1\n");
		switch(op) {
			case 1:
				//printf("Addition %d and %d is %d\n", a, b, a+b);
                                res = a+b;
                                printf("test2 = %d\n",res);
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
                        deafault: printf("invalid choice\n");
                                 break;
		}
           
                message.text[3] = res;
                child_send_flag = 1;
                message.type =2;
                printf("before child sent\n");
                int i=msgsnd(msgid, &message, sizeof(message.text), 0);
  
                printf("child sent Ret = %d message.text[3] = %d\n", i,message.text[3]);
		//return 0;
	}
	else{
		struct msgbuffer message;

		//message.type = 1;
                

		while(1)
               {
                signal(SIGINT,signal_flag);

                 if(flag_parent == 1)
                  {

		int a,b,choice;

                printf("1.add 2.sub 3.dev 4.mul\n");
		printf("Enter choice and values a, b: \n");
		scanf("%d %d %d", &choice, &a, &b);

		message.text[0] = choice;
		message.text[1] = a;
		message.text[2] = b;
		//message.text[3] = '\0';
                message.type = 1;
		int i=msgsnd(msgid, &message, sizeof(message.text), 0);
		printf("parent after sent Ret = %d\n",i);
		printf(" parent -- Queue, %d\n", msgid);
            
               // wait(NULL);
		int ret = msgrcv(msgid, &message, sizeof(message.text), 2, 0);
                printf("paren recv ret = %d\n", ret);
                printf("parent result = %d\n",message.text[3]);
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
