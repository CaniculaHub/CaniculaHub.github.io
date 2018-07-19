

ROOTFS_DIR = /opt/4412/rootfs

APP_NAME = chr_test

CROSS_COMPILE = /home/george/Linux_4412/toolchain/gcc-4.6.4/bin/arm-none-linux-gnueabi-
CC = $(CROSS_COMPILE)gcc

ifeq ($(KERNELRELEASE), )

KERNEL_DIR = /home/george/Linux_4412/kernel/linux-3.14
CUR_DIR = $(shell pwd)

all :
	make -C  $(KERNEL_DIR) M=$(CUR_DIR) modules
	$(CC) $(APP_NAME).c  -o $(APP_NAME)

clean :
	make -C  $(KERNEL_DIR) M=$(CUR_DIR) clean
	
install:
	cp -raf *.ko $(APP_NAME)   $(ROOTFS_DIR)/drv_module


else

obj-m += chr_drv.o


endif
