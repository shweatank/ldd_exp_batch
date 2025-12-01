#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/msg.h>

struct msg{
	long type;
	int num[2];
}msg;

int anum = 0;
int msqid;

void msgprint(){

	struct msg a;
	int ret = msgrcv(msqid,&a,sizeof(a.num),1,0);
	//printf("ret %d\n",ret);
	if( ret < 0){
		perror("receiving message from child failed\n");
		return;
	}

	printf("parent process: Area of square is: %d\n",a.num[1]);
}

void msgsend(){

	printf("Send success from parent\n");
	struct msg a;
	a.type = 1;
	//printf("data sent %d\n",anum);
	//anum = rand();
	anum += 2;
	a.num[0] = anum;
	printf("data sent %d\n",anum);
	if(msgsnd(msqid,&a,sizeof(a.num),0) < 0){
		perror("Sending message to child failed\n");
		return;
	}
}

int main(){
	srand(time(NULL));
	pid_t pid, ppid;
	int wrkc = 0;

	key_t key = 2708;
	msqid = msgget(key, IPC_CREAT | 0666);

//	printf("Enter next input: ");
//	scanf("%d",&anum);

	pid = fork();
	
	printf("Parent process : %d worker child : %d\n",getpid(),getppid());
	
	if(pid < 0){
		perror("Forking failed\n");
		return -1;

	}else if(pid == 0){
		//worker process
		while(1){
			struct msg a;
			
			int ret = msgrcv(msqid,&a,sizeof(a.num),1,0);
			if(ret < 0){
				perror("Receiving msg from parent failed\n");
				return -1;
			}
			printf("data rev : %d\n",a.num[0]);
			a.type = 1;
			int res = a.num[0] * a.num[0];
			a.num[1] = res;
			//printf("child : res %d\n",res);
			if(msgsnd(msqid,&a,sizeof(a.num),0) < 0){
				perror("Sending msg back to parent failed\n");
				return -1;
			}

			ppid = getppid();
			kill(ppid,SIGUSR2);
			sleep(5);
		}

	}else{
		int triggc = 0;
		int pidd   = 0;
		pidd = fork();
		printf("Parent process : %d Trigger child : %d\n",getpid(),getppid());
		
		if(pidd == 0){
			//Trigger child
			int ppid = getppid();
			while(1){
				kill(ppid,SIGUSR1);
				sleep(2);
			}

		}else{
			

			//parrent process
			signal(SIGUSR1,msgsend);
			signal(SIGUSR2,msgprint);
			//anum += 2;	

			//wait for both child process to terminate
			waitpid(triggc,0,0);
			waitpid(wrkc,0,0);
		}

	}
}
