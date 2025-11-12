#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
	pid_t pid;

	printf("pid of parent process is : %d\n",getpid());

	pid = fork();

	int a  = 5;
	printf("parrent add of a : %p\n",&a);
	printf("paraent copy\n");
	if(pid < 0){
		perror("fork failed\n");
		return 1;
	}else if(pid == 0){
		a  = 6;
		printf("add of a : %p\n",&a);
		printf("child created: child id p1 : %d parent id : %d\n",getpid(),getppid());
		//execlp("./a.out","a.out",NULL);
		//perror("execlp failed\n");
	}else{
		printf("waiting for child to finish\n");
		wait(NULL);
		printf("child process finished\n");
	}

	pid = fork();
	printf("paraent copy\n");
	if(pid < 0){
		perror("fork failed\n");
		return 1;
	}else if(pid == 0){
		printf("accessing a from p1: %d\n",a);
		printf("child created: child id p2 : %d parent id : %d\n",getpid(),getppid());
		//execlp("ls","ls","-l",NULL);
		execlp("./a.out","a.out","-l",NULL);
		//perror("execlp failed\n");
	}else{
		printf("waiting for child to finish\n");
		wait(NULL);
		printf("child process finished\n");
	}
	return 0;
}
