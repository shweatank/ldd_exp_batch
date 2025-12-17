#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>


#define DEVICE "/dev/reverse_str"

int main(){
	int fd;
	char write_buf[256];
	char read_buf[256];
	int ret;

	printf("enter string :");
	fgets(write_buf, sizeof(write_buf), stdin);
	fd = open(DEVICE, O_RDWR);
	write(fd, write_buf, strlen(write_buf));
	ret = read(fd, read_buf, sizeof(read_buf));
	read_buf[ret] = '\0';

	printf("\nreverse string : %s\n",read_buf);
	close(fd);
	return 0;
}
