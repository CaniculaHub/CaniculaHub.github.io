#ifndef __DEV_INFO_H__

#define __DEV_INFO_H__

//设置一个自定义数据，描述设备的特性
struct mydev_desc{
	char *name;
	int irqno;
	unsigned long addr;
};


#endif
