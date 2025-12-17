#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#include<stdint.h>
#include<sys/ioctl.h>
define DEV "/dev/mouse_drv"

#define MOUSE_DRV_MAGIC 'm'
#define MOUSE_DRV_CLEAR _IO(MOUSE_DRV_MAGIC, 1)
#define MOUSE_DRV_GETCNT _IOR(MOUSE_DRV_MAGIC, 2, unsigned int)
