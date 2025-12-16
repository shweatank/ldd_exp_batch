#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
 
#define GPIO "18"
#define DELAY_MS 1000
 
void write_sysfs(const char *path, const char *value) {
    int fd = open(path, O_WRONLY);
    if (fd < 0) {
        perror(path);
        exit(1);
    }
    write(fd, value, strlen(value));
    close(fd);
}
 
int main() {
    char gpio_path[64];
 
    // 1. Export GPIO
    write_sysfs("/sys/class/gpio/export", GPIO);
 
    // 2. Set direction to out
    snprintf(gpio_path, sizeof(gpio_path),
             "/sys/class/gpio/gpio%s/direction", GPIO);
    write_sysfs(gpio_path, "out");
 	
 
    // 3. LED blink loop
    snprintf(gpio_path, sizeof(gpio_path),
             "/sys/class/gpio/gpio%s/value", GPIO);
 
    printf("Blinking LED on GPIO %s...\n", GPIO);
 
    for (;;) {
        write_sysfs(gpio_path, "1");   // LED ON
    	printf("LED ON");
        usleep(DELAY_MS * 1000);
 
        write_sysfs(gpio_path, "0");   // LED OFF
    	printf("LED OFF");
        usleep(DELAY_MS * 1000);
    }
 
    return 0;
}
 
