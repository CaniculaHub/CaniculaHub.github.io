二叉树的链式存储
二叉树的链式存储结构定义示例代码如下：
typedef struct ChainTree
{
    DATA NodeData;						//元素数据
    struct ChainTree *LSonNode;			//左子树结点指针
    struct ChainTree *RSonNode;			//右子树结点指针
}ChainTreeType;
ChainTreeType *root=NULL;				//定义二叉树根结点指针
带父结点的二叉树链式存储结构定义示例代码如下：
typedef struct ChainTree
{
    DATA NodeData;						//元素数据
    struct ChainTree *LSonNode;			//左子树结点指针
    struct ChainTree *RSonNode;			//右子树结点指针
    struct ChainTree *ParentNode;		//父结点指针
}ChainTreeType;
ChainTreeType *root=NULL;				//定义二叉树根结点指针
2.7.4  准备数据
示例代码如下：
#define MANLEN 20								//最大长度
typedef char DATA;       						//定义元素类型 
typedef struct CBT  							//定义二叉树结点类型 
{
    DATA data;									//元素数据
    struct CBT *left;							//左子树结点指针
    struct CBT *right;							//右子树结点指针
}CBTType;
2.7.5  初始化二叉树
示例代码如下：
CBTType *InitTree()  							//初始化二叉树的根 
{
    CBTType *node;

    if(node=(CBTType *)malloc(sizeof(CBTType)))	//申请内存
    {
        printf("请先输入一个根结点数据:\n");
        scanf("%s",&node->data);
        node->left=NULL;
        node->right=NULL;
        if(node!=NULL) 							//如果二叉树根结点不为空 
        {
            return node;
        }
        else
        {
            return NULL;
        }
    }
    return NULL;
}
2.7.6  添加结点
添加结点的示例代码如下：
void AddTreeNode(CBTType *treeNode)			//添加结点
{
     CBTType *pnode,*parent;
     DATA data;
     char menusel;

    if(pnode=(CBTType *)malloc(sizeof(CBTType)))	//分配内存
    {
        printf("输入二叉树结点数据:\n");
        fflush(stdin);								//清空输入缓冲区 
        scanf("%s",&pnode->data);
        pnode->left=NULL; 							//设置左右子树为空 
        pnode->right=NULL;

        printf("输入该结点的父结点数据:");
        fflush(stdin);								//清空输入缓冲区 
        scanf("%s",&data);
        parent=TreeFindNode(treeNode,data);			//查找指定数据的结点 
        if(!parent)									//如果未找到
        {
            printf("未找到该父结点!\n");	
            free(pnode); 								//释放创建的结点内存 
            return;
         }
         printf("1.添加该结点到左子树\n2.添加该结点到右子树\n");
         do
        {
            menusel=getch();							//输入选择项
            menusel-='0';
            if(menusel= =1 || menusel= =2)
            {
                if(parent= =NULL)
                {
                    printf("不存在父结点，请先设置父结点!\n");
                } 
                else
                {
                    switch(menusel)
                    {
                        case 1: 						//添加到左结点 
                            if(parent->left) 			//左子树不为空 
                            {
                                printf("左子树结点不为空!\n");
                            }
                            else
                            {
                                parent->left=pnode;
                            }
                            break;
                        case 2:						//添加到右结点
                            if( parent->right) 			//右子树不为空 
                            {
                                printf("右子树结点不为空!\n"); 
                            }
                            else
                            {
                                parent->right=pnode;
                            }
                            break;
                        default:
                            printf("无效参数!\n");
                    }
                }
            }
         }while(menusel!=1 && menusel!=2);
    }
}
2.7.7  查找结点
查找结点的示例代码如下：
CBTType *TreeFindNode(CBTType *treeNode,DATA data) 	//查找结点
{
    CBTType *ptr;

    if(treeNode= =NULL)
    {
        return NULL;
    }
    else
    {
        if(treeNode->data= =data)
        {
            return treeNode; 
        }
        else
        {												//分别向左右子树递归查找 
            if(ptr=TreeFindNode(treeNode->left,data))
            {
                return ptr;
            }
            else if(ptr=TreeFindNode(treeNode->right, data)) 
            {
                return ptr;
            }
            else
            {
                return NULL; 
            }
        } 
    } 
}
2.7.8  获取左子树
获取左子树的示例代码如下：
CBTType *TreeLeftNode(CBTType *treeNode)	//获取左子树
{
    if(treeNode)
    {
        return treeNode->left;				//返回值
    }
    else
    {
        return NULL;
    }
}
2.7.9  获取右子树
获取右子树的示例代码如下：
CBTType *TreeRightNode(CBTType *treeNode) 	//获取右子树
{
    if(treeNode)
    {
        return treeNode->right;				//返回值
    }
    else
    {
        return NULL;
    }
}

2.7.10  判断空树
判断空树的示例代码如下：
int TreeIsEmpty(CBTType *treeNode)			//判断空树
{
    if(treeNode)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
2.7.11  计算二叉树深度
计算二叉树深度的示例代码如下：
int TreeDepth(CBTType *treeNode) 				//计算二叉树深度
{
    int depleft,depright;

    if(treeNode= =NULL)
    {
        return 0; 								//对于空树，深度为0
    }
    else
    {
        depleft = TreeDepth(treeNode->left); 	//左子树深度（递归调用）
        depright = TreeDepth(treeNode->right); 	//右子树深度（递归调用）
        if(depleft>depright)
        {
           return depleft + 1; 
        }
        else
        {
            return depright + 1; 
        }
    } 
}
2.7.12  清空二叉树
清空二叉树的示例代码如下：
void ClearTree(CBTType *treeNode) 				//清空二叉树
{
     if(treeNode)
     {
         ClearTree(treeNode->left); 				//清空左子树 
         ClearTree(treeNode->right);				//清空右子树 
         free(treeNode);							//释放当前结点所占内存 
         treeNode=NULL;
     }
}
2.7.13  显示结点数据
显示结点数据的示例代码如下：
void TreeNodeData(CBTType *p) 					//显示结点数据
{
     printf("%c ",p->data); 						//输出结点数据
}
2.7.14  遍历二叉树
1．按层遍历算法
按层遍历算法的示例代码如下：
void LevelTree(CBTType *treeNode,void (*TreeNodeData)(CBTType *p)) //按层遍历 
{
     CBTType *p;
     CBTType *q[MANLEN]; 						//定义一个顺序栈 
     int head=0,tail=0;

     if(treeNode)								//如果队首指针不为空     
     {
         tail=(tail+1)%MANLEN;					//计算循环队列队尾序号 
         q[tail] = treeNode;						//将二叉树根指针进队
     }
     while(head!=tail) 							//队列不为空，进行循环 
     {
         head=(head+1)%MANLEN; 					//计算循环队列的队首序号 
         p=q[head]; 								//获取队首元素 
         TreeNodeData(p);						//处理队首元素 
         if(p->left!=NULL) 						//如果结点存在左子树
         {
             tail=(tail+1)%MANLEN;				//计算循环队列的队尾序号 
             q[tail]=p->left;						//将左子树指针进队 
         }
                 
         if(p->right!=NULL)						//如果结点存在右子树 
         {
             tail=(tail+1)%MANLEN;				//计算循环队列的队尾序号 
             q[tail]=p->right;					//将右子树指针进队 
         }
     }
}
2．先序遍历算法
先序遍历算法的示例代码如下：
void DLRTree(CBTType *treeNode,void (*TreeNodeData)(CBTType *p))	//先序遍历 
{     
     if(treeNode)	
     {
         TreeNodeData(treeNode); 				//显示结点的数据 
         DLRTree(treeNode->left,TreeNodeData);
         DLRTree(treeNode->right,TreeNodeData);
     }
}
3．中序遍历算法
中序遍历算法的示例代码如下：
void LDRTree(CBTType *treeNode,void(*TreeNodeData)(CBTType *p))  	//中序遍历 
{
     if(treeNode) 
     {
         LDRTree(treeNode->left,TreeNodeData); 	//中序遍历左子树
         TreeNodeData(treeNode);					//显示结点数据 
         LDRTree(treeNode->right,TreeNodeData); 	//中序遍历右子树
     }
}
4．后序遍历算法
后序遍历算法的示例代码如下：
void LRDTree(CBTType *treeNode,void (*TreeNodeData)(CBTType *p)) 	//后序遍历
{
     if(treeNode)
     {
         LRDTree(treeNode->left,TreeNodeData); 		//后序遍历左子树 
         LRDTree(treeNode->right,TreeNodeData); 		//后序遍历右子树
         TreeNodeData(treeNode);						//显示结点数据
     }
}
