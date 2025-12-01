#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#define DEVICE "/dev/intrr"

#define MAJOR_NUM 510
#define IOCTL_GET_NUM _IOR('a', 1, int)

int main(){
	int fd;
	int val = 1234;
	int read_val = 0;
	int a, b;
	char op;
	int res;

	//printf("Enter the operation : ");
	//scanf("%c",&op);
	printf("Enter the number : ");
	scanf("%d %d",&a,&b);
	int buf[2] = {a,b};

	//printf("%d %d",buf[0],buf[1]);
	fd = open(DEVICE,O_RDWR);
	if(fd < 0){
		perror("failed to open device\n");
		return 1;
	}

	
	//
	//write input to driver using read-write system calls
	int ret = write(fd,buf,sizeof(buf));
	if(ret < 0){
		printf("write failed %d\n",ret);
		return -1;
	}
	sleep(5);
	
	//write operation-to-be-performed to driver through ioctl
	//printf("Sending value %c to kernel\n",op);
	ioctl(fd, IOCTL_GET_NUM, &res);
	
	//Reading the operation performed result from driver
	//read(fd, &res, sizeof(res));
	printf("The result is : %d\n",res);
//	printf("Requesting value %d to kernel\n",val);
//	ioctl(fd, IOCTL_GET_NUM, &read_val);
//
//	printf("Received value from kernel %d\n",read_val);
	close(fd);
}


