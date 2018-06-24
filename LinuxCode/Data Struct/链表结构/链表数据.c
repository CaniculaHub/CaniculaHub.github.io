准备数据
示例代码如下：
typedef struct
{
    char key[10];					//关键字
    char name[20];
    int age;
}Data; 	
							//数据结点类型 
typedef struct Node					//定义链表结构
{
    Data nodeData;
    struct Node *nextNode;
}CLType;

2.4.3  追加结点
在链表结构中追加结点的示例代码如下：
CLType *CLAddEnd(CLType *head,Data nodeData)	//追加结点 
{
    CLType *node,*htemp;
    if(!(node=(CLType *)malloc(sizeof(CLType))))
    {
        printf("申请内存失败！\n"); 
        return NULL;  							//分配内存失败 
    }
    else
    {
        node->nodeData=nodeData; 				//保存数据 
        node->nextNode=NULL;  					//设置结点指针为空，即为表尾 
        if(head= =NULL)  						//头指针 
        {
            head=node;
            return head;
        }
        htemp=head;
        while(htemp->nextNode!=NULL) 			//查找链表的末尾 
        {
            htemp=htemp->nextNode;
        }
        htemp->nextNode=node;
        return head;
    }
}

2.4.4  插入头结点
插入头结点的示例代码如下：
CLType *CLAddFirst(CLType *head,Data nodeData) 
{
    CLType *node;
    if(!(node=(CLType *)malloc(sizeof(CLType))))
    {
        printf("申请内存失败！\n"); 
        return NULL;  							//分配内存失败 
    }
    else
    {
        node->nodeData=nodeData; 				//保存数据 
        node->nextNode=head;  					//指向头指针所指结点 
        head=node;        						//头指针指向新增结点
        return head; 
    }
}

2.4.5  查找结点
查找结点的示例代码如下：
CLType *CLFindNode(CLType *head,char *key) 	//查找结点
{
    CLType *htemp;
    htemp=head;       							//保存链表头指针 
    while(htemp)      							//若结点有效，则进行查找 
    {
        if(strcmp(htemp->nodeData.key,key)= =0) 	//若结点关键字与传入关键字相同 
        {
            return htemp;  						//返回该结点指针 
        }
        htemp=htemp->nextNode; 					//处理下一结点 
    }
    return NULL; 								//返回空指针 
}

2.4.6  插入结点
在链表结构中插入结点的示例代码如下：
CLType *CLInsertNode(CLType *head,char *findkey,Data nodeData)  	//插入结点 
{
    CLType *node,*nodetemp;    
    if(!(node=(CLType *)malloc(sizeof(CLType))))	//分配保存结点的内容 
    {
        printf("申请内存失败！\n"); 
        return 0;  								//分配内存失败 
    }
    node->nodeData=nodeData;  					//保存结点中的数据 
    nodetemp=CLFindNode(head,findkey);
    if(nodetemp)  								//若找到要插入的结点 
    {
        node->nextNode=nodetemp->nextNode;  		//新插入结点指向关键结点的下一结点 
        nodetemp->nextNode=node;    				//设置关键结点指向新插入结点 
    }
    else
    {
        printf("未找到正确的插入位置！\n"); 
        free(node);								//释放内存
    }
    return head;								//返回头指针
}

2.4.7  删除结点
在链表结构中删除结点的示例代码如下：
int CLDeleteNode(CLType *head,char *key)
{
    CLType *node,*htemp; 						//node保存删除结点的前一结点 
    htemp=head; 
    node=head;
    while(htemp)
    {
        if(strcmp(htemp->nodeData.key,key)= =0) 	//找到关键字，执行删除操作 
        {
            node->nextNode=htemp->nextNode; 		//使前一结点指向当前结点的下一结点
            free(htemp);  						//释放内存 
            return 1;
        }
        else
        {
            node=htemp;  							//指向当前结点 
            htemp=htemp->nextNode; 				//指向下一结点 
        }
     }
     return 0;									//未删除 
}

2.4.8  计算链表长度
计算链表长度的示例代码如下：
int CLLength(CLType *head)						//计算链表长度
{
    CLType *htemp;
    int Len=0;
    htemp=head;
    while(htemp)      							//遍历整个链表 
    {
        Len++; 									//累加结点数量 
        htemp=htemp->nextNode;					//处理下一结点 
    }
    return Len;									//返回结点数量 
}

2.4.9  显示所有结点
示例代码如下：
void CLAllNode(CLType *head) 					//遍历链表 
{
    CLType *htemp;
    Data nodeData;
    htemp=head;
    printf("当前链表共有%d个结点。链表所有数据如下：\n",CLLength(head)); 
    while(htemp) 								//循环处理链表每个结点 
    {
        nodeData=htemp->nodeData;				//获取结点数据 
        printf("结点(%s,%s,%d)\n",nodeData.key,nodeData.name,nodeData.age); 
        htemp=htemp->nextNode;					//处理下一结点 
    }
}
