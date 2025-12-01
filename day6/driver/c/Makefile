obj-m=sample_driver.o
ccflags-y := -g
KDIR=/home/dell/ldd_exp_batch/day6/driver/c/linux
all:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C $(KDIR) M=$(shell pwd) modules
clean:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C $(KDIR) M=$(shell pwd) modules
