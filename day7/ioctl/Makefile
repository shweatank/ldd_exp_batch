obj-m=driverprog.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules
	sudo insmod driverprog.ko
	sudo mknod /dev/ioctl_demo c 100 0
	sudo chmod 777 /dev/ioctl_demo
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
	sudo rmmod driverprog
	sudo rm /dev/ioctl_demo
