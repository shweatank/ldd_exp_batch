#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#define DEVICE_NAME "/dev/ioctl_calc"
#define MAJOR_NUM   100
#define IOCTL_SET_OPER _IOW(MAJOR_NUM, 0, char)


int main() {
	
	int fd = open(DEVICE_NAME, O_RDWR);

	int data[2];

	printf("Enter Numbers: ");
	scanf("%d %d", data, data+1);

	write(fd, data, sizeof(data));

	
	char oper;
	printf("Enter operator +,-,*,/ : ");
	getchar();
	scanf("%c", &oper);

	printf("Sending operator %c to kernel....\n", oper);
	ioctl(fd, IOCTL_SET_OPER, &oper);

	int res;
	read(fd, &res, sizeof(res));

	printf("%d %c %d = %d\n", data[0], oper, data[1], res);

	return 0;
}
