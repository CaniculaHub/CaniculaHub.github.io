
#include <linux/init.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <asm/io.h>
#include <asm/uaccess.h>

#define GPXCON_REG 0x11000C20
#define KEY_ENTER 28

// 设计一个描述按键的数据的对象
struct key_event {
	int code; // 表示按键的类型:  home, esc, Q,W,E,R,T, ENTER
	int value; // 表示按下还是抬起 1 / 0
};

//设计一个全局设备对象--描述按键信息
struct key_desc {
	unsigned int dev_major;
	struct class *cls;
	struct device *dev;
	int irqno;
	void *reg_base;
	struct key_event event;
	wait_queue_head_t wq_head;
	int key_state; //表示是否有数据
};

struct key_desc *key_dev;

irqreturn_t key_irq_handler(int irqno, void *devid)
{
	printk("-------%s-------------\n", __FUNCTION__);

	//读取数据寄存器
	int value = readl(key_dev->reg_base + 4) & (1 << 2);

	if (value) { // 抬起
		printk("key3 up\n");
		key_dev->event.code = KEY_ENTER;
		key_dev->event.value = 0;

	} else { //按下
		printk("key3 pressed\n");
		key_dev->event.code = KEY_ENTER;
		key_dev->event.value = 1;
	}
	wake_up_interruptible(&key_dev->wq_head);
	key_dev->key_state = 1;
	return IRQ_HANDLED;
}

int get_irqno_from_node(void)
{
	// 获取到设备树中到节点
	struct device_node *np = of_find_node_by_path("/key_int_node");
	if (np) {
		printk("find node ok\n");
	} else {
		printk("find node failed\n");
	}

	// 通过节点去获取到中断号码
	int irqno = irq_of_parse_and_map(np, 0);
	printk("irqno = %d\n", irqno);

	return irqno;
}

int key_drv_open(struct inode *inode, struct file *filp)
{
	printk("-------%s-------------\n", __FUNCTION__);
	return 0;
}

ssize_t key_drv_read(struct file *filp, char __user *buf, size_t count,
		     loff_t *fpos)
{
	int ret;
	wait_event_interruptible(key_dev->wq_head, key_dev->key_state);
	ret = copy_to_user(buf, &key_dev->event, count);
	if (ret > 0) {
		printk("copy_to_user error\n");
		return -EFAULT;
	}
	


	// 传递给用户数据之后，将数据清除掉
	memset(&key_dev->event, 0, sizeof(key_dev->event));
	key_dev->key_state = 0;
	return count;
}
ssize_t key_drv_write(struct file *filp, const char __user *buf, size_t count,
		      loff_t *fpos)
{
	printk("-------%s-------------\n", __FUNCTION__);
	return 0;
}

int key_drv_close(struct inode *inode, struct file *filp)
{
	printk("-------%s-------------\n", __FUNCTION__);
	return 0;
}

const struct file_operations key_fops = {
	.open = key_drv_open,
	.read = key_drv_read,
	.write = key_drv_write,
	.release = key_drv_close,

};

static int __init key_drv_init(void)
{
	//演示如何获取到中断号
	int ret;

	// 1，设定一个全局的设备对象
	key_dev = kzalloc(sizeof(struct key_desc), GFP_KERNEL);

	// 2,申请主设备号
	key_dev->dev_major = register_chrdev(0, "key_drv", &key_fops);

	// 3,创建设备节点文件
	key_dev->cls = class_create(THIS_MODULE, "key_cls");
	key_dev->dev = device_create(
		key_dev->cls, NULL, MKDEV(key_dev->dev_major, 0), NULL, "key0");

	// 4,硬件的初始化--地址映射或者中断申请
	key_dev->irqno = get_irqno_from_node();

	ret = request_irq(key_dev->irqno, key_irq_handler,
			  IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING,
			  "key3_eint10", NULL);
	if (ret != 0) {
		printk("request_irq error\n");
		return ret;
	}

	// a，硬件如何获取数据--gpx1
	key_dev->reg_base = ioremap(GPXCON_REG, 8);
	init_waitqueue_head(&key_dev->wq_head);

	return 0;
}

static void __exit key_drv_exit(void)
{
	iounmap(key_dev->reg_base);
	free_irq(key_dev->irqno, NULL);
	device_destroy(key_dev->cls, MKDEV(key_dev->dev_major, 0));
	class_destroy(key_dev->cls);
	unregister_chrdev(key_dev->dev_major, "key_drv");
	kfree(key_dev);
}

module_init(key_drv_init);
module_exit(key_drv_exit);
MODULE_LICENSE("GPL");
