#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

struct calculator {
//  char choice;
  int a;
  int b;
  int result;
  char choice;
}calc;

int main() {
    int fd;
    //char write_buf[25];
    char read_buf[100];
   // int choice,a,b;
    ssize_t n;
     
     printf("+.add -.sub\n");
     printf("enter choice and numbers a,b\n");
     scanf("%c %d %d",&calc.choice, &calc.a, &calc.b);
     //sprintf(write_buf,"%d %d %d",choice,a,b);
 
    printf("write into file\n");
    // open file
    fd = open("/dev/calculator_dev", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    // write to file
    
//    if (write(fd, &calc, strlen(write_buf)) < 0) {
      if(write(fd, &calc, sizeof(calc)) < 0) {
        perror("write");
        close(fd);
        return 1;
    }
    close(fd);

    // reopen for reading
    fd = open("/dev/calculator_dev", O_RDONLY);
    if (fd < 0) {
        perror("open read");
        return 1;
    }

    // read from file
    //n = read(fd, read_buf, sizeof(read_buf) - 1);
     n = read(fd, &calc, sizeof(calc));
    if (n < 0) {
        perror("read");
        close(fd);
        return 1;
    }
    //read_buf[n] = '\0';

    // print data
    printf("Read using system calls:  %d\n", calc.result);

    close(fd);
    return 0;
}

