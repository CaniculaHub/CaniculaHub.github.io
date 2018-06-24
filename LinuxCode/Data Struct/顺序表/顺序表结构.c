2.3.1  准备数据
示例代码如下：
#define MAXLEN 100  					//定义顺序表的最大长度
typedef struct
{
    char key[10];  						//结点的关键字 
    char name[20];
    int age;
} DATA;    								//定义结点类型

typedef struct    						//定义顺序表结构
{
    DATA ListData[MAXLEN+1];			//保存顺序表的结构数组 
    int ListLen;              			//顺序表已存结点的数量 
}SLType;


2.3.2  初始化顺序表
示例代码如下：
void SLInit(SLType *SL) 				//初始化顺序表
{
    SL->ListLen=0;    					//初始化为空表
}


2.3.3  计算顺序表长度
示例代码如下：
int SLLength(SLType *SL)  		
{
    return (SL->ListLen);    		//返回顺序表的元素数量
}


2.3.4  插入结点
示例代码如下：
int SLInsert(SLType *SL,int n,DATA data)
{
    int i;
    if(SL->ListLen>=MAXLEN)   		//顺序表结点数量已超过最大数量 
    {
        printf("顺序表已满，不能插入结点!\n");
        return 0;             			//返回0表示插入不成功 
    }
    if(n<1 || n>SL->ListLen-1)  		//插入结点序号不正确
    {
        printf("插入元素序号错误，不能插入元素！\n");
        return 0;              		//返回0，表示插入不成功 
    } 
    for(i=SL->ListLen;i>=n;i--)  	//将顺序表中的数据向后移动 
	{
        SL->ListData[i+1]=SL->ListData[i]; 
	}
    SL->ListData[n]=data;        	//插入结点 
    SL->ListLen++;               		//顺序表结点数量增加1 
    return 1;            				//成功插入，返回1
}


2.3.5  追加结点
示例代码如下：
int SLAdd(SLType *SL,DATA data)	//增加元素到顺序表尾部
{
    if(SL->ListLen>=MAXLEN)  		//顺序表已满 
    {
        printf("顺序表已满，不能再添加结点了！\n");
        return 0;    
    }
    SL->ListData[++SL->ListLen]=data;
    return 1;
}


2.3.6  删除结点
示例代码如下：
int SLDelete(SLType *SL,int n)  		//删除顺序表中的数据元素 
{
    int i;
    if(n<1 || n>SL->ListLen)  			//删除结点序号不正确
    {
        printf("删除结点序号错误，不能删除结点！\n");
        return 0;              			//删除不成功，返回0
    } 
    for(i=n;i<SL->ListLen;i++)  		//将顺序表中的数据向前移动 
    {
        SL->ListData[i]=SL->ListData[i+1]; 
    }
    SL->ListLen--;               			//顺序表元素数量减1 
    return 1;                   			//成功删除，返回1
}


2.3.7  查找结点
1．按照序号查找结点
示例代码如下：
DATA *SLFindByNum(SLType *SL,int n)	//根据序号返回数据元素
{
    if(n<1 || n>SL->ListLen+1)  		//元素序号不正确
    {
        printf("结点序号错误，不能返回结点！\n");
        return NULL;              			//不成功，则返回0
    } 
    return &(SL->ListData[n]);
}


2．按照关键字查找结点
示例代码如下：
int SLFindByCont(SLType *SL,char *key)	//按关键字查询结点 
{
    int i;
    for(i=1;i<=SL->ListLen;i++)
    {
        if(strcmp(SL->ListData[i].key,key)= =0)	//如果找到所需结点 
        {
            return i;        						//返回结点序号 
        }
    }
    return 0;  									//搜索整个表后仍没有找到，则返回0 
}


2.3.8  显示所有结点
示例代码如下：
int SLAll(SLType *SL)  							//显示顺序表中的所有结点 
{
    int i;
    for(i=1;i<=SL->ListLen;i++)
    {
        printf("(%s,%s,%d)\n",SL->ListData[i].key,SL->ListData[i].name,SL->
ListData[i].age);
    }
    return 0;
}
