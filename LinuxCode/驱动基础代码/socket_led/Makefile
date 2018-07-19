ROOTFS_DIR = /home/linux/NFS/rootfs
MODULE_NAME = plat_led_pdev
MODULE_NAME2 = plat_led_pdrv
APP_server = server
APP_client = client
CROSS_COMPILE = /home/linux/Downloads/gcc-4.6.4/bin/arm-none-linux-gnueabi-

CC = $(CROSS_COMPILE)gcc

KERNEL_DIR =/home/linux/Exynos4412/linux-4.17 
ifeq ($(KERNELRELEASE), )


CUR_DIR = $(shell pwd)

all :
	
	make -C $(KERNEL_DIR) M=$(CUR_DIR) modules
	$(CC) $(APP_server).c -o $(APP_server)
	gcc -g -Wall $(APP_client).c -o $(APP_client)
	
clean :
	make -C $(KERNEL_DIR) M=$(CUR_DIR) clean
	rm  $(APP_server) $(APP_client) 

install:
	cp -raf *.ko $(APP_server)  $(ROOTFS_DIR)/drv_module
	
else

obj-m += $(MODULE_NAME).o
obj-m += $(MODULE_NAME2).o

endif

