obj-m=sample_driver.o
ccflags-y := -g

all:
#	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C /lib/modules/6.14.0-35-generic/build/ M=$(shell pwd) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
