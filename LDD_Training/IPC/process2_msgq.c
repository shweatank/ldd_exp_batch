#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>

struct msgbuffer {
	char text[24];
}message;

int main() {
	int msgid = 32764;
	msgrcv(msgid, &message, sizeof(message), 0, 0);
	printf("\nQueue: %d\n", msgid);
	printf("Got this message: %s\n", message.text);
	msgctl(msgid, IPC_RMID, NULL);
	return 0;
}
