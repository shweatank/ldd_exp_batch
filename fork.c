#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int main(){



	pid_t p_pid=getpid();
	printf("\nparent pid => %d\n",p_pid);


	pid_t f_pid=fork();
	if(f_pid == 0){  // Child1
		pid_t x=getpid();
		printf("\nf_pid => %d \t getpid => %d\n",f_pid,x);
		pid_t ch_pid = fork();
		if(ch_pid == 0){  // Child1's Child (C1C)
			pid_t y=getpid();
			printf("\nch_pid => %d \t  getpid => %d\n",f_pid,y);
		}
		else {
			// Child1 waiting for C1C
			wait(NULL); //  parent process to suspend its execution until one of its child processes terminates.
		}
	}
	else {
		// Parent waiting for Child1
		wait(NULL);
	}
	printf("\nprocesses id ==>> %d \n",getpid());








	/*
	pid_t p_pid=getpid();
	printf("\nparent pid => %d\n",p_pid);

	pid_t ch_pid=fork();
	printf("\nfirst child pid => %d\n",ch_pid);

	if(0 == ch_pid){
	
		pid_t ch_ch_pid = fork();

		printf("\nfirst child sub child => %d\n",ch_ch_pid);
	}

	if(getpid() == p_pid){
		pid_t ch2_pid=fork();
		printf("\nsecond child pid => %d\n",ch2_pid);
	}
	*/

}
