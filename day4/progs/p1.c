#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#define SHM_NAME "Output" 
#define SHM_SIZE 4 
int add(int a, int b){
	return a+b;
}

int sub(int a, int b){
	return a-b;
}

int mul(int a, int b){
	return a*b;
}

int div(int a, int b){
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
		res = div(a,b);
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
		int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
		if (fd == -1) {
			perror("shm_open");
			return EXIT_FAILURE;
		}

		// 2. Resize the shared memory
		if (ftruncate(fd, SHM_SIZE) == -1) {
			perror("ftruncate");
			return EXIT_FAILURE;
		}

		// 3. Map shared memory into process space
		void *ptr = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
		if (ptr == MAP_FAILED) {
			perror("mmap");
			return EXIT_FAILURE;
		}

		// 4. Write message into shared memory
		snprintf((char *)ptr, SHM_SIZE, "%d", res);
		printf("Writer: message written -> \"%d\"\n", (int *)ptr);

		// 5. Clean up
		munmap(ptr, SHM_SIZE);
		close(fd);

		wait(NULL);

	}

	printf("process F : %d\n",getpid());

	return 0;
}
