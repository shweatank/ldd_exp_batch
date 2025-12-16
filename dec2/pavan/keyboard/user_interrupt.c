#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int fd = open("/dev/interrupt_calc_dev", O_RDWR);
    if (fd < 0) {
        perror("Cannot open device");
        return 1;
    }

    int num[2], result;

    printf("Enter two numbers :\n");
    scanf("%d %d",&num[0],&num[1]);

    write(fd, &num, sizeof(num));
    

    printf("Press 'a' on the keyboard to calculate sum...\n");

    read(fd, &result, sizeof(result));
    printf("Sum of numbers = %d\n", result);

    close(fd);
    return 0;
}

