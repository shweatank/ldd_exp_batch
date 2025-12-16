#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define DEVICE		"/dev/ioctl_led_gpio"

#define MAJOR_NUM	100
#define IOCTL_SET_NUM _IOW(MAJOR_NUM, 0, int)
#define IOCTL_CLEAR_NUM _IOR(MAJOR_NUM, 1, int)

int main() {
	int fd;
	int val;

	fd = open(DEVICE, O_RDWR);
	if (fd < 0) {
		perror("Open : failed\n");
		return 1;
	}

	while(1) {
		printf("Enter num: ");
		scanf("%d", &val);
		printf("Sending value %d to kernel....\n", val);
		if(val == 1) {
			ioctl(fd, IOCTL_SET_NUM, &val);
		}
		else if(val == 0) {
			ioctl(fd, IOCTL_CLEAR_NUM, &val);
		}
		else if(val >= 2) {
			break;
		}
	}
	close(fd);

	return 0;
}
