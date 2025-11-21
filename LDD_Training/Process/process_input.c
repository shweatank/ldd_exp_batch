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

volatile int sig_usr1 = 0;
volatile int sig_usr2 = 0;
volatile int sig_term = 0;

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
	if(signum == SIGUSR1) sig_usr1 = 1;
	else if(signum == SIGUSR2) sig_usr2 = 1;
	else if(signum == SIGTERM) sig_term = 1;
}

int main() {
	
	pid_t my_id = getpid();
	printf("Parent pid: %d\n", my_id);

	struct sigaction sa;

	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = sig_handle;
	sa.sa_flags = SA_RESTART;

	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);

	sigset_t waitmask;
	sigemptyset(&waitmask);

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

		key = ftok("msgq.txt", 20);
		msgid = msgget(key, 0666 | IPC_CREAT);
		printf("Child msg ID : %d\n", msgid);
		//child that process input
		while(1) {
			sigsuspend(&waitmask);
			if(sig_usr1) {		//parent sent the data process it.
				int ret = msgrcv(msgid, &rcv_msg, sizeof(Text_t), 1, 0);
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
				ret = msgsnd(msgid, &snd_msg, sizeof(Text_t), 0);
				if (ret == 0) {
					printf("Result sent to parent\n");
					if (kill(my_id, SIGUSR2) != -1) {
						printf("Signal sent to parent\n");
					}
				}		
				}
				sig_usr1 = 0;
			}
			if(sig_term) {
				printf("Child exiting.....\n");
				return EXIT_SUCCESS;
			}
		}
	}
	else {
		struct msgbuf snd_msg;
		struct msgbuf rcv_msg;

		key_t key;
		int msgid;

		key = ftok("msgq.txt", 20);
		msgid = msgget(key, 0666 | IPC_CREAT);
		printf("Parent msg ID : %d\n", msgid);
		snd_msg.mtype = 1;

		//parent that takes input and send to child to process and receives result back
		while(1) {
			sigsuspend(&waitmask);
			if (sig_usr1) {		//parent triggered to take input
				printf("Enter two numbers: ");
				scanf("%d %d", &snd_msg.text.a, &snd_msg.text.b);
				printf("Enter the operator +-*/ : ");
				getchar();
				scanf("%c", &snd_msg.text.op);
				int ret = msgsnd(msgid, &snd_msg, sizeof(Text_t), 0);
				if(ret == 0) {
					printf("Message sent Successfully\n");
					printf("Signaling child to process input.....\n");
					if(kill(pid, SIGUSR1) != -1) {
						printf("Data sent to child\n");
					}
				}
				sig_usr1 = 0;
			}
			if(sig_usr2) {		//child sent the result print it
				int ret = msgrcv(msgid, &rcv_msg, sizeof(Text_t), 2, 0);
				if(ret) {
					printf("Result = %d\n", rcv_msg.text.a);
				}
				sig_usr2 = 0;
			}
			if(sig_term) {
				printf("Parent waiting for child to complete....\n");
				wait(NULL);
				msgctl(msgid, IPC_RMID, NULL);
				printf("Child exited. Parent exiting.....\n");
				return EXIT_SUCCESS;
			}
		}

	}
	return EXIT_SUCCESS;
}

