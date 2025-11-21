#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>

#define SHM_NAME "/calculator"
#define SHM_SIZE 1024

int main() {
	pid_t pid;

	pid = fork();

	if(pid<0) {
		perror("fork");
		return 0;
	}
	else if(pid == 0) {
	    int fd = shm_open(SHM_NAME, O_RDONLY, 0666);
    	if (fd == -1) {
        	perror("shm_open");
        	return EXIT_FAILURE;
     	}
     	void *ptr = mmap(0, SHM_SIZE, PROT_READ, MAP_SHARED, fd, 0);
     	if (ptr == MAP_FAILED) {
         perror("mmap");
         return EXIT_FAILURE;
     	}

		//Read and process memory
		int a, b;
		char ch;
		int *iptr = (int *)ptr;
		a = iptr[0];
		b = iptr[1];
		char *cptr = (char *)(iptr + 2);
		ch = cptr[0];

		switch(ch) {
			case '+':
				printf("Addition of %d and %d is %d\n", a, b, a+b);
				break;
			case '-':
				printf("Subtraction of %d and %d is %d\n", a, b, a-b);
				break;
			case '*':
				printf("Multiplication of %d and %d is %d\n", a, b, a*b);
				break;
			case '/':
				printf("Division of %d and %d is %d\n", a, b, a/b);
				break;
		}


   		 // 5. Clean up
    	 munmap(ptr, SHM_SIZE);
    	 close(fd);
		 shm_unlink(SHM_NAME);
   	     return EXIT_SUCCESS;

	}
	else {
	    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    	if (fd == -1) {
        	perror("shm_open");
        	return EXIT_FAILURE;
     	}
		if (ftruncate(fd, SHM_SIZE) == -1) {
         perror("ftruncate");
         return EXIT_FAILURE;
     	}
 

     	void *ptr = mmap(0, SHM_SIZE, PROT_READ, MAP_SHARED, fd, 0);
     	if (ptr == MAP_FAILED) {
         perror("mmap");
         return EXIT_FAILURE;
     	}

		int a, b;
		char ch;
		scanf("%d %d %c", &a, &b, &ch);
		int *iptr = (int *)ptr;
		iptr[0] = a;
		iptr[1] = b;
		char *cptr = (char *)(iptr + 2);
		cptr[0] = ch;

   		 // 5. Clean up
    	 munmap(ptr, SHM_SIZE);
    	 close(fd);
		 shm_unlink(SHM_NAME);
   	     return EXIT_SUCCESS;

	}
}

