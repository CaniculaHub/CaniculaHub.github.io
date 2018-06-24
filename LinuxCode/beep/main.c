typedef struct 
{  
    unsigned int CON;  
    unsigned int DAT;  
    unsigned int PUD; 
}GPIO;

#define  GPX2 		(*(volatile GPIO *)0x11000C40)//LED
#define  GPF3 		(* (volatile GPIO *)0x114001E0)
#define  GPD0CON		(* (volatile unsigned int *)0x114000A0)
#define  GPD0DAT		(* (volatile unsigned int *)0x114000A4)
#define  GPD0PUD 	(* (volatile unsigned int *)0x114000A8)
#define  TCFG0              (*(volatile unsigned int *)0x139D0000)
#define  TCFG1              (*(volatile unsigned int *)0x139D0004)
#define  TCNTB0             (*(volatile unsigned int *)0x139D000c)
#define  TCMPB0             (*(volatile unsigned int *)0x139D0010)
#define  TCON               (*(volatile unsigned int *)0x139D0008)

#define GPX1CON         (*(volatile unsigned int *)0x11000C20) 
#define EXT_INT41CON    (*(volatile unsigned int *)0x11000E04) 
#define EXT_INT41_MASK  (*(volatile unsigned int *)0x11000F04) 
#define EXT_INT41_PEND 	(*(volatile unsigned int *)0x11000f44)

#define ICDISER1_CPU0   (*(volatile unsigned int *)0x10490104) 
#define ICDDCR          (*(volatile unsigned int *)0x10490000)
#define ICCICR_CPU0  (*(volatile unsigned int *)0x10480000)
#define ICCPMR_CPU0  (*(volatile unsigned int *)0x10480004)
#define ICDIPTR14_CPU0 (*(volatile unsigned int *)0x10490838)
#define ICCIAR_CPU0  (*(volatile unsigned int *)0x1048000C)
#define ICDICPR1_CPU0 (*(volatile unsigned int *)0x10490284)
#define ICCEOIR_CPU0 (*(volatile unsigned int *)0x10480010)
unsigned int flag=0;
void bsp_init()//设置4个LED管脚为输出模式
{
    GPX2.CON = GPX2.CON & (~(0xf0000000)) | 0x10000000;  //led
	GPF3.CON = GPF3.CON & (~(0x000f0000)) | 0x00010000;//led
	GPX1CON=(GPX1CON&~(0x0F<<4))|(0x0F<<4); //set gpx1_1 interrupt mode  
	GPD0CON = (GPD0CON &~(0x0f)) | (0x02);
}

void  delay_ms(unsigned int ms)  
{  
    unsigned int i,j;
	for (i = 0; i < ms; i++)
	for (j = 0; j < 2000; j++); 
}  



void interrupt_init(void)
{
  EXT_INT41CON =(EXT_INT41CON&~(0x07<<4))|(0x02<<4);//  set GPX1_1 falling edge triger  
  EXT_INT41_MASK = (EXT_INT41_MASK &~(0x01<<1)); //set pin interrupt enable 
  ICDISER1_CPU0 = ICDISER1_CPU0 | (1<<25);	//EINT9 (GPX1_1)  GIC中断使能
  ICDIPTR14_CPU0 = 0x01010101; 
  ICDDCR = ICDDCR|1; //GIC 分发总使能
  ICCICR_CPU0 = 1;  // CPU0  中断使能
  ICCPMR_CPU0 = 0XFF;   //设置CPU0的优先级门槛为最低

  GPX1CON = (GPX1CON&~(0x0f<<8)) | (0x0f<<8);  //设置GPX1_2为中断模式
  EXT_INT41CON = (EXT_INT41CON&~(0x07<<8)) | (0x02<<8); //设置为下降沿触发
  EXT_INT41_MASK = EXT_INT41_MASK&~(0x01<<2);
  ICDISER1_CPU0 = ICDISER1_CPU0 | (1<<26);  //设置EXT10（GPX1_2） GIC中断使能
}

void pwm_close()
{
	TCON = (TCON&~(0x0f)) | 0x08; 
}
void pwm_on()
{
	//2.设置功能块
	//a.设置输出625HZ
	TCFG0 = (TCFG0&~(0xff)) | 100;  //一级分平 100
	TCFG1 = (TCFG1&~(0x0f)) | 0x03;  //二级分平 8
	TCNTB0 = 200;  //三级分平200
	TCMPB0 = 100;  

	//TCON = (TCON&~(0x0f)) | 0x0b; 要先分平再启动
	TCON = (TCON&~(0x0f)) | 0x0a; 
	TCON = (TCON&~(0x0f)) | 0x09; 
}
void do_irq(void )
{
	 int irq_num;
	  irq_num = ICCIAR_CPU0&0x3ff; ////�ж�ID��
	  switch(irq_num)
	  {
      case 57:
		  flag=1;
		  pwm_on();
		  EXT_INT41_PEND = EXT_INT41_PEND|(1<<1);  //��GPX1_1�жϱ�־
		  ICDICPR1_CPU0 = ICDICPR1_CPU0|(1<<25);    //��GIC GPX1_1�жϱ�־
		  break;
		  case 58:
			pwm_close();
			EXT_INT41_PEND = EXT_INT41_PEND | (1<<2);
			ICDICPR1_CPU0 = ICDICPR1_CPU0 | (1<<26);
      default:
		 
		  break;
	  }
	  ICCEOIR_CPU0 = (ICCEOIR_CPU0&0x3FF)|irq_num;    //�����ж�	��������ɵ��ж�ID��д��üĴ��������ʾ��Ӧ���жϴ������
}


int main(int argc, char const *argv[])
{
	/* code */
	interrupt_init();
	bsp_init();
	while(1)
	{
		if(flag==1)
		{
		GPX2.DAT = GPX2.DAT|0x80;  
        delay_ms(500);
        GPX2.DAT = GPX2.DAT&(~(0x80));   
        delay_ms(500);
		}
	
	}
	return 0;
}
