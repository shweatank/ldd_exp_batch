#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(){
	
	char s[50];
	printf("Enter text: ");
	scanf("%s",s);

	int fd = open("text.txt", O_CREAT | O_RDWR,0666);
	if(fd < 0){
		perror("opening file failed\n");
		return -1;
	}

	int bytes = write(fd, s, strlen(s));
	printf("number of bytes received: %d\n",bytes);
	close(fd);


	fd = open("text.txt", O_CREAT | O_RDWR,0666);
	if(fd < 0){
		perror("opening file failed\n");
		return -1;
	}
	char ptr[50];
	int read_bytes = read(fd,ptr,sizeof(ptr) - 1);
	printf("bytes read : %d string: %s",read_bytes,ptr);
	close(fd);
}
