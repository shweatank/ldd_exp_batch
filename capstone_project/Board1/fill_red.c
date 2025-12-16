#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#define DEV_NODE "/dev/ili9225"

#define WIDTH  176
#define HEIGHT 220

int main(void)
{
	int fd;
	size_t frame_size = WIDTH * HEIGHT * 2;
	uint8_t *frame;

	/* Open device */
	fd = open(DEV_NODE, O_WRONLY);
	if (fd < 0) {
		perror("open /dev/ili9225");
		return 1;
	}

	/* Allocate frame buffer */
	frame = malloc(frame_size);
	if (!frame) {
		perror("malloc");
		close(fd);
		return 1;
	}

	/* RGB565 RED = 0xF800 */
	/* RGB565 RED = 0xF800 (byte-swapped) */
	for (size_t i = 0; i < frame_size; i += 2) {
		frame[i]     = 0x00;  /* LOW byte first */
		frame[i + 1] = 0xF8;  /* HIGH byte */
	}


	/* Write full frame */
	ssize_t ret = write(fd, frame, frame_size);
	if (ret < 0) {
		perror("write");
	} else if (ret != frame_size) {
		fprintf(stderr, "Partial write: %zd bytes\n", ret);
	} else {
		printf("Screen filled with RED\n");
	}

	free(frame);
	close(fd);
	return 0;
}

