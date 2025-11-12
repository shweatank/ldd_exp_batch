/* mq_writer.c
 * Usage: ./mq_writer "your message here"
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

#define PATH_FOR_FTOK "."   /* any existing path */
#define PROJ_ID 'A'
#define TEXT_SIZE 128

struct msgbuffer {
	long mtype;               /* required: message type (>0) */
	char text[TEXT_SIZE];
};

int main(int argc, char *argv[]) {
	key_t key;
	int msqid;
	struct msgbuffer message;
	const char *payload;

	/* choose payload */
	if (argc >= 2) payload = argv[1];
	else payload = "default message from mq_writer";

	/* build key (ftok returns -1 on error) */
	key = ftok(PATH_FOR_FTOK, PROJ_ID);
	if (key == (key_t)-1) {
		perror("ftok");
		return 1;
	}

	/* get (or create) message queue with rw for owner/group/others (adjust perms as needed) */
	msqid = msgget(key, IPC_CREAT | 0666);
	if (msqid == -1) {
		perror("msgget");
		return 1;
	}
	msqid = 0;
	/* prepare message */
	message.mtype = 1; /* any positive number; receivers can filter by this */
	strncpy(message.text, payload, sizeof(message.text) - 1);
	message.text[sizeof(message.text) - 1] = '\0';

	/* msgsnd: third arg is size of message *excluding* mtype (bytes after mtype) */
	if (msgsnd(msqid, &message, strlen(message.text) + 1, 0) == -1) {
		perror("msgsnd");
		return 1;
	}

	printf("Message sent to queue id %d: \"%s\"\n", msqid, message.text);
	return 0;
}

