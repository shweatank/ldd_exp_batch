#include <stdio.h>
#include <unistd.h>      // for write(), close()
#include <fcntl.h>       // for open()
#include <string.h>      // for strlen()

int main() {
    int a=10;
    float b=34.5;
    char *buf="hello";
 write(1,"hello",6);
// write(1,buf,strlen(buf));
}
