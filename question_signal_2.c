#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<string.h>
#include<sys/wait.h>
struct message{

	long type;
	char string[100];
};

int msgid;

void pa_send(){

	printf("\nsingnal_1\n");
	struct message msg;
	msg.type = 1;
	strcpy(msg.string, "hello coder");
	msgsnd(msgid, &msg, sizeof(msg.string), 0);
	printf("\nmessage send successfully\n");
}
void pa_rcv(){
	struct message msg;
	msgrcv(msgid, &msg, sizeof(msg.string), 2, 0);
	printf("\nthis message send by child ==>> %s\n",msg.string);

}
int main(){
	printf("pid => %d\n",getpid());


	key_t key = ftok("progfile", 65);
	msgid = msgget(key, 0666 | IPC_CREAT);

	if(msgid == -1){
	
		perror("msgget");
		exit(EXIT_FAILURE);
	}


	pid_t pid = fork();

	if(pid < 0){
	
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if(pid == 0){
		struct message msg;
		msgrcv(msgid, &msg, sizeof(msg.string), 1, 0);

		printf("\nchild proccess : %s\n", msg.string);

		msg.type=2;
		strcpy(msg.string, "this message from child");
		msgsnd(msgid, &msg, sizeof(msg.string), 0);
		printf("\nchild send the msg\n");
		kill(pid, SIGUSR2)


	}
	else{
		signal(SIGUSR1, pa_send);
		signal(SIGUSR2, pa_rcv);
		while(1){
			printf("\nparent proccess");
			sleep(2);
		}
	}
	wait(NULL);

	return 0;
}
