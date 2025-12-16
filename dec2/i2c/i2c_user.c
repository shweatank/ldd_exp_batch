#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define DEVICE_PATH "/dev/oled_char"

int main(int argc, char *argv[])
{
    int fd;
    ssize_t ret;
    char buf[128];

    if (argc < 2) {
        printf("Usage: %s <string_to_display>\n", argv[0]);
        return 1;
    }

    fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0) {
        perror("Failed to open device");
        return 1;
    }

    // Combine all arguments into one string
    buf[0] = '\0';
    for (int i = 1; i < argc; i++) {
        strcat(buf, argv[i]);
        if (i < argc - 1)
            strcat(buf, " "); // add space between words
    }

    // Write string to OLED
    ret = write(fd, buf, strlen(buf));
    if (ret < 0) {
        perror("Failed to write to device");
        close(fd);
        return 1;
    }

    printf("Written to OLED: %s\n", buf);

    // Optional: read status from OLED driver
    ret = read(fd, buf, sizeof(buf)-1);
    if (ret < 0) {
        perror("Failed to read from device");
        close(fd);
        return 1;
    }
    buf[ret] = '\0';
    printf("Device says: %s", buf);

    close(fd);
    return 0;
}

