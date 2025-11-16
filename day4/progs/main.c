#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>	
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>

struct msgbuffer {
    long mtype;    // message type must be > 0
    int value;     // integer message
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
		
	pipe(fd);
	pid_t pid;
	printf("parent process id : %d\n",getpid());

	pid = fork();

	if(pid < 0){
		perror("Error while forking process\n");
		return -1;
	}else if(pid == 0){

		close(fd[1]);
		int result;
		read(fd[0],&result,sizeof(result));
		printf("child process is : %d parent process id : %d\n",getpid(),getppid());
		printf("res of calculation is : %d\n",result);
		close(fd[0]);
	
	}else{
		close(fd[0]);
		if(write(fd[1],&res,sizeof(res)) == -1 )
			perror("write error");
		
		wait(NULL);
	
	}
	printf("process Finished : %d\n",getpid());

	return 0;
}
