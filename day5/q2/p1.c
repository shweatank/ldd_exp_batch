#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/msg.h>

struct msg{
	long type;
	int num;
	int res;
}msg;

int anum = 0;
int msqid;

void msgprint(){

	struct msg a;

	if(msgrcv(msqid,&a,sizeof(a.res),1,0) < 0){
		perror("receiving message from child failed\n");
		return;
	}

	printf("parent process: Area of square is: %d\n",a.res);
}

void msgsend(){
	printf("send success\n");
	struct msg a;
	printf("data sent %d\n",anum);
	a.num = anum;
	if(msgsnd(msqid,&a,sizeof(a.num),0) < 0){
		perror("Sending message to child failed\n");
		return;
	}
}

int main(){

	pid_t pid, ppid;
	int wrkc = 0;

	key_t key = 2708;
	msqid = msgget(key, IPC_CREAT | 0666);
	printf("parent pid : %d",getpid());	

	pid = fork();
	
	if(pid < 0){
		perror("Forking failed\n");
		return -1;
	}else if(pid == 0){
		//worker process
		while(1){
			wrkc = getpid();
			struct msg a;
			int ret = msgrcv(msqid,&a,sizeof(a.num),1,0);

			if(ret < 0){
				perror("Receiving msg from parent failed\n");
				return -1;
			}

			int res = a.num * a.num;
			a.res = res;
			printf("child : res %d\n",res);
			if(msgsnd(msqid,&a,sizeof(a.res),0) < 0){
				perror("Sending msg back to parent failed\n");
				return -1;
			}
			ppid = getppid();
			kill(ppid,SIGUSR2);
		}

	}else{
		int triggc = 0;
		int pidd   = 0;
		pidd = fork();
		
		if(pidd == 0){
			//Trigger child
			while(1){
				triggc = getpid();
				printf("Enter next input: ");
				scanf("%d",&anum);
				if(anum == -1){
					break;
				}
				int pp = getppid();
				kill(pp,SIGUSR1);
				sleep(2);
			}

		}else{
			//parrent process
			signal(SIGUSR1,msgsend);
			signal(SIGUSR2,msgprint);
			
			//wait for both child process to terminate
			waitpid(triggc,0,0);
			waitpid(wrkc,0,0);
		}

	}
}
