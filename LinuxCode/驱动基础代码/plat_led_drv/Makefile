ROOTFS_DIR = /opt/4412/rootfs
MODULE_NAME = plat_led_pdev
MODULE_NAME2 = plat_led_pdrv
APP_NAME = led_test

CROSS_COMPILE = /home/linux/Downloads/gcc-4.6.4/bin/arm-none-linux-gnueabi-
CC = $(CROSS_COMPILE)gcc
ifeq ($(KERNELRELEASE), )
KERNEL_DIR =/home/linux/Exynos4412/linux-4.17 
CUR_DIR = $(shell pwd)
all :
	$(CC) $(MODULE_NAME).c -o $(MODULE_NAME)
	make -C $(KERNEL_DIR) M=$(CUR_DIR) modules
clean :
	make -C $(KERNEL_DIR) M=$(CUR_DIR) clean

install:
	cp -raf *.ko $(APP_NAME)  $(ROOTFS_DIR)/drv_module
	
else

obj-m += $(MODULE_NAME).o
obj-m += $(MODULE_NAME2).o

endif
