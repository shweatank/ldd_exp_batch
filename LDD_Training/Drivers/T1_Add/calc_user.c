#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

struct data {
	int n1;
	int n2;
	char op;
};

int main() {

	struct data Data;

	printf("Enter two number: ");
	scanf("%d %d", &Data.n1, &Data.n2);

	printf("Enter op +,-,*,/ : ");
	getchar();
	scanf("%c", &Data.op);

	int fd = open("/dev/calc_char_dev", O_RDWR);

	write(fd, &Data, sizeof(struct data));

	int res;
	read(fd, &res,sizeof(res));

	printf("%d %c %d = %d\n", Data.n1, Data.op, Data.n2, res);
	return 0;
}
