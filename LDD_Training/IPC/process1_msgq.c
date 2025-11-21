#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>

struct msgbuffer {
	char text[24];
}message;

int main() {
	int msgid = 32764;
	strcpy(message.text, "opensource.com");
	int i=msgsnd(msgid, &message, sizeof(message), 0);
	printf("Ret = %d\n", i);
	printf("Message: %s\n", message.text);
	printf("Queue, %d\n", msgid);
	return 0;
}
