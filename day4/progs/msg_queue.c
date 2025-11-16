#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>	
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>

struct msgbuffer {
    long mtype;   
    int value;     
};

int add(int a, int b){
	return a+b;
}

int sub(int a, int b){
	return a-b;
}

int mul(int a, int b){
	return a*b;
}

int divv(int a, int b){
	return a/b;
}

int main(){

	int a, b;
	int op = 0;
	int res = 0;
	int fd[2];
	printf("Enter the nummber : ");
	scanf("%d %d",&a,&b);

	printf("Enter the operation to be performed: ");
	scanf("%d",&op);
	
	if(op == 1){
		res = add(a,b);
	}else if(op == 2){
		res = sub(a,b);
	}else if(op == 3){
		res = mul(a,b);
	}else if(op == 4){
		res = divv(a,b);
	}
		
	//pipe(fd);
	pid_t pid;
	printf("parent process id : %d\n",getpid());

	key_t key = 1234; // or use ftok("somepath", 'A')
	int msqid = msgget(key, IPC_CREAT | 0666);
	if (msqid == -1) {
		perror("msgget");
		return 1;
	}

	struct msgbuffer message;
	message.mtype = 1; // message type > 0
	//strncpy(message.text, "deva the best", sizeof(message.text));
	//message.text[sizeof(message.text)-1] = '\0';
	message.value = res;
	/* msgsnd returns 0 on success, -1 on error.
	   Pass the size of the text *only* (excluding mtype). */
	if (msgsnd(msqid, &message, sizeof(message.value), 0) == -1) {
		perror("msgsnd");
		return 1;
	}

	pid = fork();

	if(pid < 0){
		perror("Error while forking process\n");
		return -1;
	}else if(pid == 0){

		struct msgbuffer message;
		ssize_t r = msgrcv(msqid, &message, sizeof(message.value), 1, 0);
		if (r == -1) {
			perror("msgrcv");
			_exit(1);
		}
		printf("Child: received value = %d (bytes=%zd)\n", message.value, r);

		/*close(fd[1]);
		int result;
		read(fd[0],&result,sizeof(result));
		printf("child process is : %d parent process id : %d\n",getpid(),getppid());
		printf("res of calculation is : %d\n",result);
		close(fd[0]);*/
	
	}else{
		/*close(fd[0]);
		if(write(fd[1],&res,sizeof(res)) == -1 )
			perror("write error");
		*/
		wait(NULL);
		if (msgctl(msqid, IPC_RMID, NULL) == -1) {
			perror("msgctl(IPC_RMID)");
			return 1;
		}
	
	}
	printf("process F : %d\n",getpid());

	return 0;
}
