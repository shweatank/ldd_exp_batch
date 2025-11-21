#include <stdio.h>
#include <string.h>

int main() {
	FILE *fd;
	fd = fopen("./hello.txt", "w");

	char *ptr = "This is Swathi";
	fwrite((void *)ptr, strlen(ptr), 1, fd);

	fclose(fd);

	fd = fopen("./hello.txt", "r");
	char str[100];
	fread(str, strlen(ptr), 1, fd);
	printf("Read : %s\n", str);

	fclose(fd);
	return 0;

}
