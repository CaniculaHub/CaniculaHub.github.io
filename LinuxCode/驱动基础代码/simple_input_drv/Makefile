ROOTFS_DIR = /home/linux/NFS/rootfs

MODULE_NAME = simple_input
APP_NAME = simple_input_test

CROSS_COMPILE = /home/linux/Downloads/gcc-4.6.4/bin/arm-none-linux-gnueabi-

CC = $(CROSS_COMPILE)gcc

KERNEL_DIR =/home/linux/Exynos4412/linux-4.17 
ifeq ($(KERNELRELEASE), )


CUR_DIR = $(shell pwd)

all :
	
	make -C  $(KERNEL_DIR) M=$(CUR_DIR) modules
	$(CC) $(APP_NAME).c  -o $(APP_NAME)
	
clean :
	make -C  $(KERNEL_DIR) M=$(CUR_DIR) clean
	rm -rf $(APP_NAME) 

install:
	cp -raf *.ko $(APP_NAME)   $(ROOTFS_DIR)/drv_module
	
else

obj-m += $(MODULE_NAME).o

endif

