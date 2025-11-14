/*Problem Statement
---------------------------------------------------------------------------------
write a program where use signal to trigger parent process to copy data using ( message queue ) send it to child process which is in wait state for data then it does calculation and send data back to parent and send signal to parent so parent can print it ( until signal is not received it should be waiting for result to print ) so we have three processes here
--------------------------------------------------------------------------------
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

int sig_int = 0;
int sig_term = 0;
int sig_tstp = 0;

typedef struct text {
	int a;
	int b;
	char op;
}Text_t;

struct msgbuf {
	long mtype;
	Text_t text;	
};

void sig_handle(int signum) {
	if(signum == SIGINT) sig_int = 1;
	else if(signum == SIGTERM) sig_term = 1;
	else if(signum == SIGTSTP) sig_tstp = 1;
}

int main() {
	
	pid_t my_id = getpid();
	printf("Parent pid: %d\n", my_id);

	signal(SIGINT, sig_handle);
	signal(SIGTERM, sig_handle);
	signal(SIGTSTP, sig_handle);

	pid_t pid = fork();
	printf("Child pid: %d\n", pid);

	if(pid < 0) {
		perror("fork");
		return EXIT_FAILURE;
	}
	else if(pid == 0) {
		struct msgbuf rcv_msg;
		struct msgbuf snd_msg;
		key_t key;
		int msgid;

		//key = ftok("file.c", 65);
		key = 20;
		msgid = msgget(key, 0666);
		printf("Child msg ID : %d\n", msgid);
		//child that process input
		while(1) {
			if(sig_int) {		//parent sent the data process it.
				int ret = msgrcv(msgid, &rcv_msg, sizeof(rcv_msg), 1, 0);
				printf("Received %d %c %d = ....\n", rcv_msg.text.a, rcv_msg.text.op, rcv_msg.text.b);
				snd_msg.text.a = 0;
				if(ret) {
				switch(rcv_msg.text.op) {
					case '+':
						snd_msg.text.a = rcv_msg.text.a + rcv_msg.text.b;
						break;
					case '-':
						snd_msg.text.a = rcv_msg.text.a - rcv_msg.text.b;
						break;
					case '*':
						snd_msg.text.a = rcv_msg.text.a * rcv_msg.text.b;
						break;
					case '/':
						snd_msg.text.a = rcv_msg.text.a / rcv_msg.text.b;
						break;
				}
				snd_msg.mtype = 2;
				ret = msgsnd(msgid, &snd_msg, sizeof(snd_msg), 0);
				if (ret == 0) {
					printf("Result sent to parent\n");
					if (kill(my_id, SIGTSTP) != -1) {
						printf("Signal sent to parent\n");
					}
				}		
				}
				sig_int = 0;
			}
			if(sig_term) {
				msgctl(msgid, IPC_RMID, NULL);
				printf("Child exiting.....\n");
				exit(1);
			}
		}
	}
	else {
		struct msgbuf snd_msg;
		struct msgbuf rcv_msg;

		key_t key;
		int msgid;

		//key = ftok("file.c", 65);
		key = 20;
		msgid = msgget(key, 0666 | IPC_CREAT);
		printf("Parent msg ID : %d\n", msgid);
		snd_msg.mtype = 1;

		//parent that takes input and send to child to process and receives result back
		while(1) {
			if (sig_int) {		//parent triggered to take input
				printf("Enter two numbers: ");
				scanf("%d %d", &snd_msg.text.a, &snd_msg.text.b);
				printf("Enter the operator +-*/ : ");
				getchar();
				scanf("%c", &snd_msg.text.op);
				int ret = msgsnd(msgid, &snd_msg, sizeof(snd_msg), 0);
				if(ret == 0) {
					printf("Message sent Successfully\n");
					printf("Signaling child to process input.....\n");
					if(kill(pid, SIGINT) != -1) {
						printf("Data sent to child\n");
					}
				}
				sig_int = 0;
			}
			if(sig_tstp) {		//child sent the result print it
				int ret = msgrcv(msgid, &rcv_msg, sizeof(rcv_msg), 2, 0);
				if(ret) {
					printf("Result = %d\n", rcv_msg.text.a);
				}
				sig_tstp = 0;
			}
			if(sig_term) {
				printf("Parent waiting for child to complete....\n");
				wait(NULL);
				msgctl(msgid, IPC_RMID, NULL);
				printf("Child exited. Parent exiting.....\n");
				exit(1);
			}
		}

	}
	return EXIT_SUCCESS;
}

