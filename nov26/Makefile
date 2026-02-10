obj-m = kmem_leak.o

KDIR := /home/mirafra/practice/rasberrypi/linux
CROSS_COMPILE = aarch64-linux-gnu-
ARCH = arm64

all:
	make -C $(KDIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE)  modules

clean:
	make -C $(KDIR) M=$(PWD) clean

