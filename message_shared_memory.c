#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<unistd.h>
#include<sys/wait.h>

#define memory_size 1024 // memory size

int main(){


	key_t key = ftok("shM_file", 'R');
	// Generate a key for the shared memory segment
	// A filename → "shM_file" ( shM_file should be an existing file )
	// A project ID (1 character) → 'R'
	
	int shMid = shmget(key, memory_size, IPC_CREAT | 0666);
	// Create the shared memory segment return ID
	// IPC_CREAT → shared memory segment Create if not already present 
	// 0666 → Read + write permission for everyone (rw-rw-rw-)

	if(shMid == -1){
	
		perror("shmget");
		exit(EXIT_FAILURE);
	}

	char *shM_ptr = (char *)shmat(shMid, NULL, 0);
	// Attach the shared memory segment to the parent's address space
	// NULL  ->  attach at any available address
	// 0  ->  Read + Write access is allowed
	

	pid_t pid = fork();
	if(pid < 0){
	
		perror("fork");
	}
	else if(pid == 0){

		printf("\nchild process :\t%s\n",shM_ptr);

		if(shmdt(shM_ptr) == -1){
		
			perror("smdt child");
			exit(EXIT_FAILURE);
		}

	}
	else{
	
		printf("\nwriting in shared memory\n");
		const char *message = "hello this message from parent";
		strncpy(shM_ptr, message, memory_size);
		shM_ptr[memory_size-1] = '\0';

		wait(NULL);

		// Detach the shared memory segment
		if(shmdt(shM_ptr) == -1){
		
			perror("shmdt parent");
			exit(EXIT_FAILURE);
		}

		// Remove the shared memory segment
		if(shmctl(shMid, IPC_RMID, NULL) == -1){
		
			perror("shmctl IPC_RMID");
			exit(EXIT_FAILURE);
		}
		printf("\nparent process : shared memory removed\n");
	}

}
