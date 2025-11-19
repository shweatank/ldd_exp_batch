/**
* super_simple_userspace_driver_struct.c
*
* Same simple open/read/write/close "driver",
	* but now the APIs are stored in a struct and called via that struct.
	*/

#include <stdio.h>
#include <string.h>

	/* -------------------------------
	   Device: simple buffer + state
	   ------------------------------- */
	char device_buffer[64];
	int  device_is_open = 0;

	/* -------------------------------
	   Driver API functions
	   ------------------------------- */

int my_open(void)
{
	if (device_is_open) {
		printf("Device already open!\n");
		return -1;
	}
	device_is_open = 1;
	printf("Device opened\n");
	return 0;
}

int my_write(const char *data)
{
	if (!device_is_open) {
		printf("Device not open!\n");
		return -1;
	}

	strncpy(device_buffer, data, sizeof(device_buffer) - 1);
	device_buffer[sizeof(device_buffer) - 1] = '\0';

	printf("Write: '%s'\n", device_buffer);
	return 0;
}

int my_read(char *out)
{
	if (!device_is_open) {
		printf("Device not open!\n");
		return -1;
	}

	printf("Read : '%s'\n", device_buffer);
	strcpy(out, device_buffer);
	return 0;
}

int my_close(void)
{
	if (!device_is_open) {
		printf("Device already closed!\n");
		return -1;
	}
	device_is_open = 0;
	printf("Device closed\n");
	return 0;
}

/* -------------------------------
   Struct holding the APIs
   (like a tiny file_operations)
   ------------------------------- */

struct driver_ops {
	int (*open)(void);
	int (*write)(const char *data);
	int (*read)(char *out);
	int (*close)(void);
};

/* Initialize the ops table */
struct driver_ops my_driver = {
	.open  = my_open,
	.write = my_write,
	.read  = my_read,
	.close = my_close
};

/* -------------------------------
   Demo / test
   ------------------------------- */

int main(void)
{
	char temp[64];

	/* All calls now go via the struct */
	my_driver.open();
	my_driver.write("hello driver via struct");
	my_driver.read(temp);
	my_driver.close();

	return 0;
}
