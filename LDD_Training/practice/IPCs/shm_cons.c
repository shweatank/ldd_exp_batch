#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>

int main() {

	int fd = shm_open("/my_shm", O_CREAT | O_RDWR, 0666);

	ftruncate(fd, 1024);

	char *ptr = mmap(NULL, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	printf("Read = %s\n", ptr);
	munmap(ptr, 1024);

	shm_unlink("/my_shm");
	return 0;
}
