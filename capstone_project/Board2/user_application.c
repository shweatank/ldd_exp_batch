#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <termios.h>

/* IOCTL */
#define IR_MAGIC        'i'
#define IR_GET_STATUS   _IOR(IR_MAGIC, 1, int)

#define UART_DEV "/dev/my_uart0"   // change if needed

int main(void)
{
    int ir_fd, uart_fd;
    int status = 0;
    struct termios uart_cfg;

    /* Open IR sensor device */
    ir_fd = open("/dev/ir_sensor", O_RDWR);
    if (ir_fd < 0) {
        perror("IR open");
        return 1;
    }

    /* Open UART */
    uart_fd = open(UART_DEV, O_RDWR | O_NOCTTY);
    if (uart_fd < 0) {
        perror("UART open");
        close(ir_fd);
        return 1;
    }

    int count = 0;
    printf("Monitoring IR sensor...\n");

    while (1) {
        ioctl(ir_fd, IR_GET_STATUS, &status);

      /*     if((count % 5) == 0)
		   status = 1;
	   else 
		   status = 0; */

        if (status == 1) {
	    char msg = '1';
            printf("Object detected! Sending to UART...%d\n",++count);
	   // printf("count = %d - %c\n",count++, msg);
            write(uart_fd, &msg,1);

           sleep(5);
	   // int n = read(uart_fd,&msg,1);
	   // printf("receive data = %c\n",msg);
        }
	else{
	    char msg = '0';
	    //printf("%c\n",msg);
            write(uart_fd, &msg,1);
         //sleep(3);
	 printf("not detected %d\n",++count);
        // int n = read(uart_fd,&msg,1);
	// printf("receive data = %c\n",msg);

        }

	count ++;
	//sleep(1);
    }

    close(uart_fd);
    close(ir_fd);
    return 0;
}

