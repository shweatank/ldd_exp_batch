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

volatile int sig_usr1 = 0;
volatile int sig_usr2 = 0;
volatile int sig_term = 0;

void sig_handle(int signum)
 {
	if(signum == SIGUSR1) 
		sig_usr1 = 1;
	else if(signum == SIGUSR2) 
		sig_usr2 = 1;
	else if(signum == SIGTERM) 
		sig_term = 1;
}

int main() {
       
	printf(" pid = %d\n",getpid());
       key_t key;
       int msgid;
       key = ftok("file.c", 65);
       msgid = msgget(key, 0666 | IPC_CREAT);
 	
	signal(SIGUSR1, sig_handle);
	signal(SIGUSR2, sig_handle);
	signal(SIGTERM, sig_handle);

	pid_t pid = fork();

	if(pid < 0) {
		perror("fork");
		return -1;
	}
	else if(pid == 0) {
		
		while(1){
               
	        if(sigusr1 == 1){
                       
		struct msgbuffer message;
		message.type=1;
                printf("before child recv %d\n",getpid());
		if((msgrcv(msgid, &message,sizeof(message.text),message.type, 0)==-1));
                  {
                    perror("child msgrcv error");
        //           exit(1);
 		   }
		int a,b, op;
		a = message.text[1];
		b = message.text[2];
		op = message.text[0];
		switch(op) {
			case 1:
				//printf("Addition %d and %d is %d\n", a, b, a+b);
                                res = a+b;
                                printf("test2 = %d\n",res);
				break;
			
			case 2:  res = a-b;
				//printf("Subtraction %d and %d is %d\n", a, b, a-b);
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
                message.type =2;
                printf("before child sent\n");
                int i=msgsnd(msgid, &message, sizeof(message.text), 0);
  
                printf("child sent Ret = %d message.text[3] = %d\n", i,message.text[3]);
                }
		}
	}
	else{
		struct msgbuffer message;

		while(1)
               {

                 if(sig_usr1 == 1)
                  {
		int a,b,choice;

                printf("1.add 2.sub 3.dev 4.mul\n");
		printf("Enter choice and values a, b: \n");
		scanf("%d %d %d", &choice, &a, &b);

		message.text[0] = choice;
		message.text[1] = a;
		message.text[2] = b;
                
		message.type = 1;
		int i=msgsnd(msgid, &message, sizeof(message.text), 0);
		printf("parent after sent Ret = %d\n",i);
            	sig_usr1 = 0;
                

		int ret = msgrcv(msgid, &message, sizeof(message.text), 2, 0);
                printf("paren recv ret = %d\n", ret);
                printf("parent result = %d\n",message.text[3]);
                 msgctl(msgid, IPC_RMID, NULL);
                flag_parent = 0;
                 
                    }		           
                }
		
		}
	return 0;
}
