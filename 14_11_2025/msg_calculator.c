#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

struct msgbuffer {
	long type;
	char text[24];
}message;

int main() {

	pid_t pid = fork();

	if(pid < 0) {
		perror("fork");
		return -1;
	}
	else if(pid == 0) {
		struct msgbuffer message;
		key_t key;
		int msgid;

		// ftok to generate unique key
		key = 10;
		// msgget creates a message queue
		// and returns identifier
		msgid = msgget(key, 0666 | IPC_CREAT);

		// msgrcv to receive message
		int ret = msgrcv(msgid, &message, sizeof(message), 1, 0);
		printf("ret = %d\n", ret);
		int a,b, op;
		a = (int)message.text[0];
		b = (int)message.text[4];
		op = message.text[8];
		switch(op) {
			case '+':
				printf("Addition %d and %d is %d\n", a, b, a+b);
				break;
			case '-':
				printf("Subtraction %d and %d is %d\n", a, b, a-b);
				break;
			case '*':
				printf("Subtraction %d and %d is %d\n", a, b, a*b);
				break;
			case '/':
				printf("Subtraction %d and %d is %d\n", a, b, a/b);
				break;
		}

		// to destroy the message queuehar a,b,c;
		msgctl(msgid, IPC_RMID, NULL);
		return 0;
	}
	else {
		struct msgbuffer message;
		key_t key;
		int msgid;

		// ftok to generate unique key
		key = 10;
		// msgget creates a message queue
		// and returns identifier
		msgid = msgget(key, 0666 | IPC_CREAT);
		message.type = 1;

		int a,b;
		char c;
		printf("Enter a b and op: ");
		scanf("%d %d %c", &a, &b, &c);

		message.text[0] = a;
		message.text[4] = b;
		message.text[8] = c;
		message.text[3] = '\0';

		int i=msgsnd(msgid, &message, sizeof(message), 0);
		printf("Ret = %d\n", i);
		printf("Message: %s\n", message.text);
		printf("Queue, %d\n", msgid);
		wait(NULL);
	}
	return 0;
}
