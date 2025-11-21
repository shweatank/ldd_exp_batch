#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#define DEVICE_NAME "/dev/interrupt_calc"


int main() {
	
	int fd = open(DEVICE_NAME, O_RDWR);

	int data[2];

	printf("Enter Numbers: ");
	scanf("%d %d", data, data+1);

	write(fd, data, sizeof(data));


	int res;
	do {
		read(fd, &res, sizeof(res));
	}while(res == 0);

	printf("Result = %d\n", res);

	return 0;
}
