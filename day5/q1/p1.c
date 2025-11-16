#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>

typedef struct Info{
	long mtype;
	int a[3];
}Info;

int msqid;

void senddata(){
	printf("hi\n");
	Info data = {1,{1,2,0}};
	int ret = msgsnd(msqid, &data, sizeof(data.a), 0);
	if(ret < 0){
		perror("Send failed\n");
		return;
	}

}

void printdata(){
	Info data;
	int ret = msgrcv(msqid, &data, sizeof(data.a),1, 0);
	if(ret < 1){
		perror("receive from child failed\n");
		return;
	}
	printf("parent: The result of calculation is : %d\n",data.a[2]);
}

int main(){
	int ppid = 0;
	int pid = 0;
	
	//Created key id for message queue
	key_t key = 1234; 
        msqid = msgget(key, IPC_CREAT | 0666);
        if (msqid == -1) {
                perror("msgget");
                return 1;
        }

	ppid = getpid();
	printf("parent process id : %d\n",ppid);
	pid = fork();

	if(pid < 0){
		
		perror("fork() Failed\n");
		return 1;
	
	}else if (pid == 0){
		//Child process	
		Info data;
		
		int ret = msgrcv(msqid, &data, sizeof(data.a),1,0);
		if(ret < 0){
			perror("receiving message from parent failed\n");
			exit(0);
		}
		
		int result = data.a[0] + data.a[1];
		data.a[2] = result;
		printf("Child: The result of addition is %d\n",result);
		
		ret = msgsnd(msqid,&data,sizeof(data.a),0);
		if(ret < 0){
			perror("sending message back to parent failed \n");
			exit(0);
		}

		kill(ppid,SIGCONT);

		
	}else{
		signal(SIGINT,senddata);
		signal(SIGCONT,printdata);
		//parent process
		wait(NULL);

	}

	//while(1){};
}
