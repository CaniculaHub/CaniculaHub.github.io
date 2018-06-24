队列结构操作示例
【程序示例2-4】使用队列结构实现学生数据操作
程序示例代码如下：
#include <stdlib.h>							//头文件
#include <stdio.h>
#include <string.h>

#define QUEUELEN 15							//最大长度

typedef struct
{
    char name[10];
    int age;
}DATA;

typedef struct
{
    DATA data[QUEUELEN]; 					//队列数组 
    int head; 								//队头 
    int tail; 								//队尾 
}SQType;

SQType *SQTypeInit()
{
SQType *q;

    if(q=(SQType *)malloc(sizeof(SQType))) 	//申请内存 
    {	
        q->head = 0;							//设置队头 
        q->tail = 0;							//设置队尾 
        return q;
    }
    else
    {
        return NULL; 						//返回空
    }
}

int SQTypeIsEmpty(SQType *q)  				//判断空队列
{
    int temp;
    temp=q->head= =q->tail;
    return (temp);
}

int SQTypeIsFull(SQType *q)				//判断满队列
{
    int temp;
    temp=q->tail= =QUEUELEN;
    return (temp);
}

void SQTypeClear(SQType *q)  				//清空队列 
{
    q->head = 0;								//设置队头 
    q->tail = 0;								//设置队尾 
}

void SQTypeFree(SQType *q) 					//释放队列
{
    if (q!=NULL)
    {
        free(q);
    }
}

int InSQType(SQType *q,DATA data)			//入队列
{	
    if(q->tail= =QUEUELEN)
    { 
        printf("队列已满!操作失败!\n");
        return(0);
    }
    else
    {
        q->data[q->tail++]=data;				//将元素入队列
        return(1);
    }
}

DATA *OutSQType(SQType *q) 					//出队列
{
    if(q->head= =q->tail)
    {
        printf("\n队列已空!操作失败!\n");
        exit(0);
    }
    else
    {
        return &(q->data[q->head++]);
    }
}

DATA *PeekSQType(SQType *q) 				//读结点数据
{
    if(SQTypeIsEmpty(q))
    {
        printf("\n空队列!\n");
        return NULL; 
    }
    else
    {
        return &(q->data[q->head]);
    }
}

int SQTypeLen(SQType *q) 					//计算队列长度 
{
    int temp;
    temp=q->tail-q->head;
    return (temp);
}

void main()									//主函数
{
    SQType *stack;
    DATA data;
    DATA *data1;    

    stack=SQTypeInit();  					//初始化队列
    printf("入队列操作：\n");
    printf("输入姓名 年龄进行入队列操作:"); 
    do
    {
        scanf("%s%d",data.name,&data.age);
        if(strcmp(data.name,"0")= =0) 
        {
            break; 							//若输入0，则退出
        }
        else
        {
            InSQType(stack,data);
        }
    }while(1);

    do
    {
        printf("出队列操作:按任意键进行出栈操作:\n");
        getchar();
        data1=OutSQType(stack);
        printf("出队列的数据是(%s,%d)\n" ,data1->name,data1->age);	
    }while(1);
    SQTypeFree(stack); 						//释放队列所占用的空间 
}
