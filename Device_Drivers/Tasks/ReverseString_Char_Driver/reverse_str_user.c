#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

int main() {

	char Data[100];

	printf("Enter Word: ");
	scanf("%s", Data);

	int fd = open("/dev/reverse_str_char_dev", O_RDWR);

	write(fd, &Data, strlen(Data));

	int len = strlen(Data);
	char out[len+1];

	read(fd, &out,len);
	out[len] = '\0';

	printf("Reverse of %s = %s\n", Data, out);
	return 0;
}
