#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define DRIVER_NAME "/dev/mathops"

int main(){
	
	int a[3];
	printf("Enter the numbers : ");
	scanf("%d %d",&a[0],&a[1]);

	printf("Enter the operation you want to perfom.\n1.Add\n2.Sub\n3.Mul\n4.Div\n");
	scanf("%d",&a[2]);

	int fd = open(DRIVER_NAME,O_RDWR);	
	
	write(fd, a, sizeof(a));
	
	int result = 0;
	read(fd,&result, sizeof(result));
	printf("The result of calculation : %d\n",result);
	close(fd);
}
