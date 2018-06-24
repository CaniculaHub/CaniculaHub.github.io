准备数据
示例代码如下：
#define QUEUELEN 15

typedef struct
{
    char name[10];
    int age;
}DATA;

typedef struct
{
    DATA data[QUEUELEN]; 						//队列数组 
    int head; 									//队头 
    int tail; 									//队尾 
}SQType;

2.6.3  初始化队列结构
初始化顺序队列的示例代码如下：
SQType *SQTypeInit()
{
    SQType *q;
    if(q=(SQType *)malloc(sizeof(SQType))) 		//申请内存 
    {
        q->head = 0;								//设置队头 
        q->tail = 0;								//设置队尾 
        return q;
    }
    else
    {
        return NULL; 							//返回空
    }
}

2.6.4  判断空队列
判断空队列就是判断一个队列结构是否为空。如果是空队列，则表示该队列结构中没有数据。此时可以进行入队列操作，但不可以进行出队列操作。
判断空队列的示例代码如下：
int SQTypeIsEmpty(SQType *q)  					//判断空队列
{
    int temp;
    temp=q->head= =q->tail;
    return (temp);
}
在这里，输入参数q为一个指向操作的队列的指针。程序中，根据队列head是否等于tail，判断队列是否为空。

2.6.5  判断满队列
判断满队列的示例代码如下：
int SQTypeIsFull(SQType *q)					// 判断满队列
{
    int temp;
    temp=q->tail= =QUEUELEN;
    return (temp);
}
在这里，输入参数q为一个指向操作的队列的指针。程序中，判断队列tail是否已等于符号常量QUEUELEN，从而判断队列是否已满。

2.6.6  清空队列
清空队列就是清除队列中的所有数据。清空队列的示例代码如下：
void SQTypeClear(SQType *q)  					//清空队列 
{
    q->head = 0;									//设置队头 
    q->tail = 0;									//设置队尾 
}
在这里，输入参数q为一个指向操作的队列的指针。程序中，简单地将队列顶指针head和tail设置为0，表示执行清空队列操作。

2.6.7  释放空间
释放空间的示例代码如下：
void SQTypeFree(SQType *q) 						//释放队列
{
    if (q!=NULL)
    {
        free(q);
    }
}

2.6.8  入队列
入队列操作的示例代码如下：
int InSQType(SQType *q,DATA data)				//入队列
{
    if(q->tail= =QUEUELEN)
    { 
        printf("队列已满!操作失败!\n");
        return(0);
    }
    else
    {
        q->data[q->tail++]=data;					//将元素入队列
        return(1);
    }
}

2.6.9  出队列
出队列的具体实现代码如下：
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

2.6.10  读结点数据
读结点数据的代码示例如下：
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

2.6.11  计算队列长度
计算队列长度的示例代码如下：
int SQTypeLen(SQType *q) 					//计算队列长度 
{
    int temp;
    temp=q->tail-q->head;
    return (temp);
}
