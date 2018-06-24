准备数据
示例代码如下：
#define MAXLEN 50
typedef struct
{
    char name[10];
    int age;
}DATA;

typedef struct stack
{
    DATA data[SIZE+1]; 					//数据元素 
    int top; 							//栈顶 
}StackType;

2.5.3  初始化栈结构
初始化顺序栈的示例代码如下：
StackType *STInit()
{
    StackType *p;
    
    if(p=(StackType *)malloc(sizeof(StackType))) 	//申请栈内存 
    {
        p->top=0; 									//设置栈顶为0 
        return p;									//返回指向栈的指针 
    }
    return NULL;
}

这里首先使用malloc()函数申请内存，申请成功后设置栈顶为0，然后返回申请内存的首地址。如果申请内存失败，将返回NULL。
2.5.4  判断空栈
判断空栈的示例代码如下：
int STIsEmpty(StackType *s) 						//判断栈是否为空 
{
    int t;
    t=(s->top= =0);
    return t;
}

2.5.5  判断满栈
判断满栈的示例代码如下：
int STIsFull(StackType *s) 							//判断栈是否已满
{
    int t;
    t=(s->top= =MAXLEN);
    return t;
}

2.5.6  清空栈
清空栈就是栈中的所有数据被清除。清空栈的示例代码如下：
void STClear(StackType *s)  					//清空栈 
{
    s->top=0;
}

2.5.7  释放空间
释放空间的示例代码如下：
void STFree(StackType *s) 						//释放栈所占用空间 
{
    if(s)
    {
        free(s);
    }
}

2.5.8  入栈
入栈操作的示例代码如下：
int PushST(StackType *s,DATA data)				//入栈操作 
{
     if((s->top+1)>MAXLEN)
     {
         printf("栈溢出!\n"); 
         return 0;
     }
     s->data[++s->top]=data;						//将元素入栈
     return 1; 
}

2.5.9  出栈
DATA PopST(StackType *s) 						//出栈操作 
{
     if(s->top= =0)
     {
         printf("栈为空!\n");
         exit(0);
     }
     return (s->data[s->top--]);
}

2.5.10  读结点数据
读结点数据的示例代码如下：
DATA PeekST(StackType *s) 						//读栈顶数据
{
     if(s->top= =0)
     {
         printf("栈为空!\n");
         exit(0);
     }
     return (s->data[s->top]);
}
