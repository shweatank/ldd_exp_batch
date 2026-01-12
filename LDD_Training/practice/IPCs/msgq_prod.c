#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct msg {
	long msg_type;
	char data[100];
};

int main() {
	
	struct msg msg;
	msg.msg_type = 1;
	int id = msgget(12345, IPC_CREAT | 0666);

	strcpy(msg.data, "Hello..");

	msgsnd(id, &msg, 8, 0);

	printf("Sent = %s\n", msg.data);

	return 0;

}	
