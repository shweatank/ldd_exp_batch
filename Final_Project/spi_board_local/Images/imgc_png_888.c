#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define DEV_NODE "/dev/ili9225"

#define WIDTH   176
#define HEIGHT  220
#define FRAME_SIZE (WIDTH * HEIGHT * 2)

int main(int argc, char *argv[])
{
    int img_fd, dev_fd;
    uint8_t *buf;
    ssize_t ret;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s image.rgb\n", argv[0]);
        return 1;
    }

    /* Open image file */
    img_fd = open(argv[1], O_RDONLY);
    if (img_fd < 0) {
        perror("open image");
        return 1;
    }

    /* Allocate buffer */
    buf = malloc(FRAME_SIZE);
    if (!buf) {
        perror("malloc");
        close(img_fd);
        return 1;
    }

    /* Read image */
    ret = read(img_fd, buf, FRAME_SIZE);
    if (ret != FRAME_SIZE) {
        fprintf(stderr, "Image size must be exactly %d bytes\n", FRAME_SIZE);
        close(img_fd);
        free(buf);
        return 1;
    }
    close(img_fd);

    /* BYTE SWAP for your driver (BIG → LITTLE endian) */
    for (size_t i = 0; i < FRAME_SIZE; i += 2) {
        uint8_t tmp = buf[i];
        buf[i] = buf[i + 1];
        buf[i + 1] = tmp;
    }

    /* Open device */
    dev_fd = open(DEV_NODE, O_WRONLY);
    if (dev_fd < 0) {
        perror("open /dev/ili9225");
        free(buf);
        return 1;
    }

    /* Write frame */
    ret = write(dev_fd, buf, FRAME_SIZE);
    if (ret < 0) {
        perror("write");
    } else if (ret != FRAME_SIZE) {
        fprintf(stderr, "Partial write: %zd bytes\n", ret);
    } else {
        printf("Image displayed successfully\n");
    }

    close(dev_fd);
    free(buf);
    return 0;
}

