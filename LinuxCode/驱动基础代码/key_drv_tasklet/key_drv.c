#include <linux/init.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/sched.h>
#include <linux/poll.h>

#include <asm/io.h>
#include <asm/uaccess.h>

#define GPXCON_REG  0x11000C20
#define KEY_ENTER		28

// ���һ���������������ݵĶ���
struct key_event{
	int code; // ��ʾ����������:  home, esc, Q,W,E,R,T, ENTER
	int value; // ��ʾ���»���̧�� 1 / 0
};

//���һ��ȫ���豸����--����������Ϣ
struct key_desc{
	unsigned int dev_major;
	struct class *cls;
	struct device *dev;
	int irqno;
	void *reg_base;
	struct key_event event;
	wait_queue_head_t  wq_head;
	int key_state; //��ʾ�Ƿ�������
	struct fasync_struct *faysnc;
	struct tasklet_struct mytasklet;
	
};

struct key_desc *key_dev;


void key_tasklet_half_irq(unsigned long data)
{
	printk("-------%s-------------\n", __FUNCTION__);
	// ��ʾ������,��Ҫȥ������������/�ȴ�����
	wake_up_interruptible(&key_dev->wq_head);
	//ͬʱ���ñ�־λ
	key_dev->key_state  = 1;

	//�����ź�

	kill_fasync(&key_dev->faysnc, SIGIO, POLLIN);
	
}


irqreturn_t key_irq_handler(int irqno, void *devid)
{
	printk("-------%s-------------\n", __FUNCTION__);

	//��ȡ���ݼĴ���
	int value = readl(key_dev->reg_base + 4) & (1<<2);
	if(value){ // ̧��
		printk("key3 up\n");
		key_dev->event.code = KEY_ENTER;
		key_dev->event.value = 0;

	}else{//����
		printk("key3 pressed\n");
		key_dev->event.code = KEY_ENTER;
		key_dev->event.value = 1;
	}
	// �����°벽
	tasklet_schedule(&key_dev->mytasklet);
	
	return IRQ_HANDLED;
}

int get_irqno_from_node(void)
{
	// ��ȡ���豸���е��ڵ�
	struct device_node *np = of_find_node_by_path("/key_int_node");
	if(np){
		printk("find node ok\n");
	}else{
		printk("find node failed\n");
	}

	// ͨ���ڵ�ȥ��ȡ���жϺ���
	int irqno = irq_of_parse_and_map(np, 0);
	printk("irqno = %d\n", irqno);
	
	return irqno;
}

int key_drv_open(struct inode *inode, struct file *filp)
{

	printk("-------%s-------------\n", __FUNCTION__);
	return 0;
}

ssize_t key_drv_read(struct file *filp, char __user *buf, size_t count, loff_t *fpos)
{
	//printk("-------%s-------------\n", __FUNCTION__);

	//�����ǰ�Ƿ�����ģʽ������û�����ݣ�������һ��������
	if(filp->f_flags & O_NONBLOCK && !key_dev->key_state)
		return -EAGAIN;
	
	int ret;
	//2������Ҫ�ȴ�(û������)��ʱ�򣬽�������
	wait_event_interruptible(key_dev->wq_head, key_dev->key_state);
	
	// ��ʾ������
	ret = copy_to_user(buf, &key_dev->event,  count);
	if(ret > 0)
	{
		printk("copy_to_user error\n");
		return -EFAULT;
	}

	// ���ݸ��û�����֮�󣬽����������
	memset(&key_dev->event, 0,  sizeof(key_dev->event));
	key_dev->key_state = 0;
	
	return count;
	
}
ssize_t key_drv_write(struct file *filp, const char __user *buf, size_t count, loff_t *fpos)
{
	printk("-------%s-------------\n", __FUNCTION__);
	return 0;

}

int key_drv_close (struct inode *inode, struct file *filp)
{
	printk("-------%s-------------\n", __FUNCTION__);
	return 0;

}

unsigned int key_drv_poll(struct file *filp, struct poll_table_struct *pts)
{
	// ����һ��maskֵ
	unsigned int mask;
	// ����poll_wait,����ǰ���ȴ�����ע��ϵͳ��
	poll_wait(filp, &key_dev->wq_head, pts);
	
	// 1����û�����ݵ�ʱ�򷵻�һ��0
	if(!key_dev->key_state)
		mask = 0;

	// 2�������ݷ���һ��POLLIN
	if(key_dev->key_state)
		mask |= POLLIN;

	return mask;
}

int key_drv_fasync(int fd, struct file *filp, int on)
{
	//ֻ��Ҫ����һ��������¼�źŸ÷��͸�˭
	return fasync_helper(fd, filp, on,  &key_dev->faysnc);

}

const struct file_operations key_fops = {
	.open = key_drv_open,
	.read = key_drv_read,
	.write = key_drv_write,
	.release = key_drv_close,
	.poll = key_drv_poll,
	.fasync = key_drv_fasync,
	
};

static int __init key_drv_init(void)
{
	//��ʾ��λ�ȡ���жϺ�
	int ret;

	// 1���趨һ��ȫ�ֵ��豸����
	key_dev = kzalloc(sizeof(struct key_desc),  GFP_KERNEL);
	
	// 2,�������豸��
	key_dev->dev_major = register_chrdev(0, "key_drv", &key_fops);

	// 3,�����豸�ڵ��ļ�
	key_dev->cls = class_create(THIS_MODULE, "key_cls");
	key_dev->dev = device_create(key_dev->cls, NULL, 
									MKDEV(key_dev->dev_major,0), NULL, "key0");


	// 4,Ӳ���ĳ�ʼ��--��ַӳ������ж�����
	key_dev->irqno = get_irqno_from_node();

	ret = request_irq(key_dev->irqno, key_irq_handler, IRQF_TRIGGER_FALLING|IRQF_TRIGGER_RISING, 
					"key3_eint10", NULL);
	if(ret != 0)
	{
		printk("request_irq error\n");
		return ret;
	}

	// a��Ӳ����λ�ȡ����--gpx1
	key_dev->reg_base  = ioremap(GPXCON_REG, 8);

	// ��ʼ���ȴ�����ͷ
	init_waitqueue_head(&key_dev->wq_head);

	//��ʼ��tasklet
	tasklet_init(&key_dev->mytasklet, key_tasklet_half_irq, 45);
	
	
	return 0;
}	


static void __exit key_drv_exit(void)
{

	tasklet_kill(&key_dev->mytasklet);
	
	iounmap(key_dev->reg_base);
	free_irq(key_dev->irqno, NULL);
	device_destroy(key_dev->cls, MKDEV(key_dev->dev_major,0));
	class_destroy(key_dev->cls);
	unregister_chrdev(key_dev->dev_major, "key_drv");
	kfree(key_dev);

}

module_init(key_drv_init);
module_exit(key_drv_exit);
MODULE_LICENSE("GPL");

