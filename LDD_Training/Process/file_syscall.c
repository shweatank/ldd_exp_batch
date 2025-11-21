#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

int main() {
	int	fd = open("./out.txt", O_CREAT | 0666);

	write(fd, "Hello", 6);

	close(fd);

}
