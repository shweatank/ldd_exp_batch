#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>

int main(){
	char write_buf[256];
	char read_buf[256];

	int fd;
	int ret;
	fd=open("/dev/file_operation", O_RDWR);
	if(fd < 0){
		perror("failed to open\n");
		return 1;
	}

	printf("write the message\n");
	fgets(write_buf, sizeof(write_buf), stdin);
	ret = write(fd, write_buf, strlen(write_buf));

	if(ret < 0){
		perror("write failed\n");
		close(fd);
		return 1;
	}
	lseek(fd, 0, SEEK_SET);
	ret = read(fd, read_buf, sizeof(read_buf));
	read_buf[ret]='\0';
	printf("\nret = %d\nmsg = %s\n", ret, read_buf);
	close(fd);
	return 0;
}
