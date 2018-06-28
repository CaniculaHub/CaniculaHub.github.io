ROOTFS_DIR = /opt/4412/rootfs
MODULE_NAME = mybus
MODULE_NAME2 = mydev
MODULE_NAME3 = mydrv
APP_NAME = led_test

CROSS_COMPILE = /home/linux/Downloads/gcc-4.6.4/bin/arm-none-linux-gnueabi-
CC = $(CROSS_COMPILE)gcc

KERNEL_DIR =/home/linux/Exynos4412/linux-4.17 
ifeq ($(KERNELRELEASE), )
CUR_DIR = $(shell pwd)
all :
	make -C $(KERNEL_DIR) M=$(CUR_DIR) modules
	#$(CC) $(MODULE_NAME)_test.c -o $(MODULE_NAME)_test
clean :
	make -C $(KERNEL_DIR) M=$(CUR_DIR) clean

install:
	cp -raf *.ko   $(ROOTFS_DIR)/drv_module
	
else

obj-m += $(MODULE_NAME).o
obj-m += $(MODULE_NAME2).o
obj-m += $(MODULE_NAME3).o

endif
