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

	msgrcv(id, &msg, 8, msg.msg_type,0);

	printf("Received = %s\n", msg.data);

	msgctl(id, IPC_RMID, NULL);
	return 0;

}	
