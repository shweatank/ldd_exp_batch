#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringSerial.h>

int main()
{
    int serial_port;
    char data_to_send[] = "Hello from RPi 1!\n";
    int baud_rate = 9600;
    
    if (wiringPiSetup() == -1) {
        fprintf(stderr, "Unable to start wiringPi: %s\n", strerror(errno));
        return 1;
    }

    if ((serial_port = serialOpen("/dev/serial0", baud_rate)) < 0) {
        fprintf(stderr, "Unable to open serial device: %s\n", strerror(errno));
        return 1;
    }

    printf("Serial port opened at %d baud.\n", baud_rate);
    
    while(1)
    {
        serialPuts(serial_port, data_to_send);
        printf("Sent: %s", data_to_send);
        
        delay(1000);
    }
    return 0;
}