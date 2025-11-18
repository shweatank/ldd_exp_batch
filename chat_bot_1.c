#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

struct msgbuffer {
	long type;
	char text[50];
}message;

struct msgbuffer snd_msg;
struct msgbuffer recv_msg;

int snd_msgid, rcv_msgid;
pthread_t send_th, recv_th;

int running = 1;
void *send_thread(void *ptr) {
	while(running) {
		printf("A: ");
		fgets(snd_msg.text,50,stdin);
		snd_msg.type = 1;
		int ret = msgsnd(snd_msgid, &snd_msg, sizeof(snd_msg.text), 0);
		if(ret == -1) printf("Message NOT sent....!\n");
		if(strcmp(snd_msg.text, "exit\n") == 0) {
			running = 0;
			break;
		}
	}
	pthread_exit(NULL);
}

void *receive_thread(void *ptr) {
	while(running) {
		int ret = msgrcv(rcv_msgid, &recv_msg, sizeof(recv_msg.text), 1, IPC_NOWAIT);
		if (ret > 0) {
			printf("\rB: %sA: ", recv_msg.text);
			fflush(stdout);
			if(strcmp(recv_msg.text, "exit\n") == 0) {
				running = 0;
				break;
			}
		}
	}
	pthread_exit(NULL);
}


int main() {

	int snd_key, rcv_key;
	// ftok to generate unique key
	snd_key = ftok("bot.txt", 65);
	rcv_key = ftok("msg.txt", 40);

	// msgget creates a message queue
	// and returns identifier
	snd_msgid = msgget(snd_key, 0666 | IPC_CREAT);
	rcv_msgid = msgget(rcv_key, 0666 | IPC_CREAT);

	int ret = pthread_create(&send_th, NULL, send_thread, NULL);
	if(ret != 0) {
		perror("pthread");
		return -1;
	}
	ret = pthread_create(&recv_th, NULL, receive_thread, NULL);
	if(ret != 0) {
		perror("pthread");
		return -1;
	}

	pthread_join(send_th, NULL);
	pthread_join(recv_th, NULL);

	msgctl(snd_msgid, IPC_RMID, NULL);
	msgctl(rcv_msgid, IPC_RMID, NULL);
	return 0;

}