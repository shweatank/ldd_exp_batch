/*
 * super_simple_userspace_driver.c
 *
 * The simplest userspace program that mimics a Linux char driver.
 *
 * It shows:
 *  - open()
 *  - read()
 *  - write()
 *  - close()
 *
 * Just like kernel file_operations, but extremely simplified.
 */

#include <stdio.h>
#include <string.h>

/* -------------------------------
   Device: simple buffer + state
   ------------------------------- */
char device_buffer[64];
int device_is_open = 0;

/* -------------------------------
   Fake driver operations
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
   Demo / test
   ------------------------------- */
int main(void)
{
    char temp[64];

    my_open();               // open device
    my_write("hello driver");// write
    my_read(temp);           // read
    my_close();              // close

    return 0;
}
