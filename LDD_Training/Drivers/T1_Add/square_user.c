#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>


int main() {

	int Data;

	printf("Enter number: ");
	scanf("%d", &Data);

	int fd = open("/dev/square_char_dev", O_RDWR);

	write(fd, &Data, sizeof(Data));

	int res;
	read(fd, &res,sizeof(res));

	printf("Square of %d = %d\n", Data, res);
	return 0;
}
