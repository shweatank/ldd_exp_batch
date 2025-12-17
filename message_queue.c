#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>

struct msg_str{

	long msg_type;
	// msg_type is mandatory in System V message queues.
	// You cannot remove it.
	// System V message queues allow many different messages to be stored in the same queue at the same time.
	char msg_text[100];
};

int main(){

	key_t key = ftok("progfile", 65);
	//we need a common unique key for IPC between processes because Both get the same key, so they connect to the same message queue/shared memory.
	
	int msgid;
	pid_t pid;
	msgid = msgget(key, 0666 | IPC_CREAT);
	//msgget = creating message queue
	// 0666 = (This is permission bits (like file permissions).)
	// IPC_CRET = If the queue exists → open it otherwise create it)
	
	if(msgid == -1){
		perror("msgget");
		exit(EXIT_FAILURE);
	}

	pid = fork();

	if(pid < 0){
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if(pid == 0){
		struct msg_str message;
		msgrcv(msgid, &message, sizeof(message.msg_text), 1, 0);
		// 1 => Receive only messages whose msg_type = 1
		// 0 => block until message arrives
		printf("\nchild first msg :\t%s\n",message.msg_text);

		struct msg_str message_2;
		msgrcv(msgid, &message_2, sizeof(message_2.msg_text), 2, 0);
		printf("\nchild second msg :\t%s\n",message_2.msg_text);

		exit(EXIT_SUCCESS);
	}
	else{
		struct msg_str message;
		message.msg_type = 1; // Message type
		strcpy(message.msg_text, "hello from parent process");
		msgsnd(msgid, &message, sizeof(message.msg_text),0);
		// This parameter tells the OS how many bytes of the message to send (excluding msg_type)
		// 0 -> Normal behavior (wait if queue full)  OR  IPC_NOWAIT Do NOT wait if queue is full → return error immediately


		printf("\nfirst parent sent :\t%s\n",message.msg_text);


		// ## second message send
		
		struct msg_str message_2;
		message_2.msg_type = 2;
		strcpy(message_2.msg_text, "this is second message from parent process");
		msgsnd(msgid, &message_2, sizeof(message_2.msg_text), 0);
		printf("\nsecond parent sent :\t%s\n",message_2.msg_text);



		wait(NULL);
		// Wait for child to finish

		msgctl(msgid, IPC_RMID, NULL);
		// Remove the message queue

		exit(EXIT_SUCCESS);
	}
	
	return 0;
}
