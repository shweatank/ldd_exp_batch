#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <stdlib.h>

struct msgbuffer {
	long mtype;         // required
	char text[24];
};

int main() {
	key_t key = 1234; // or use ftok("somepath", 'A')
	int msqid = msgget(key, IPC_CREAT | 0666);
	if (msqid == -1) {
		perror("msgget");
		return 1;
	}

	struct msgbuffer message;
	message.mtype = 1; // message type > 0
	strncpy(message.text, "deva the best", sizeof(message.text));
	message.text[sizeof(message.text)-1] = '\0';

	/* msgsnd returns 0 on success, -1 on error.
	   Pass the size of the text *only* (excluding mtype). */
	if (msgsnd(msqid, &message, sizeof(message.text), 0) == -1) {
		perror("msgsnd");
		return 1;
	}

	printf("sent successful\n");
	printf("message : %s\n", message.text);
	printf("queue id: %d\n", msqid);
	return 0;
}

