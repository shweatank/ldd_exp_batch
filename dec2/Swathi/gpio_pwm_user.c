#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#define MAJOR 100
#define IOCTL_SET_DT _IOW(MAJOR, 0, int)

int main() {
	int fd = open("/dev/ioctl_led_pwm", O_RDWR);

	int duty = 50; // 50%
	while(1) {
		printf("Enter duty cycle: ");
		scanf("%d", &duty);
		ioctl(fd, IOCTL_SET_DT, &duty);
	}
	close(fd);
}
