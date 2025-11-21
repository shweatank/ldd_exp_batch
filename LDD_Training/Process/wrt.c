#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
	int x = 32;
	char str[20];
	sprintf(str, "%d", x);
	write(1, str, 3);
	float f = 2.001;
	sprintf(str, "%f", f);
	write(1, str, 6);
	char *buf = "hello";
	write(1, buf, 6);
}
