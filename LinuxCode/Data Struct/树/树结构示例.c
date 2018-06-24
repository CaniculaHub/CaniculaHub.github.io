树结构操作示例
【程序示例2-5】4顶点无向图操作演示
程序示例代码如下：
#include <stdio.h> 
#include <stdlib.h> 
#include <conio.h>

#define MANLEN 20								//最大长度
typedef char DATA;       						//定义元素类型 
typedef struct CBT  							//定义二叉树结点类型 
{
    DATA data;									//元素数据
    struct CBT *left;							//左子树结点指针
    struct CBT *right;							//右子树结点指针
}CBTType;

//二叉树操作函数

void main() 									//主函数
{
    CBTType *root=NULL; 						//root为指向二叉树根结点的指针 
    char menusel;
    void (*TreeNodeData1)(); 					//指向函数的指针 
    TreeNodeData1=TreeNodeData; 				//指向具体操作的函数 
    //设置根元素
    root=InitTree();
    //添加结点
    do{
        printf("请选择菜单添加二叉树的结点\n");
        printf("0.退出\t");						//显示菜单
        printf("1.添加二叉树的结点\n");
        menusel=getch();
        switch(menusel)
        {
            case '1':							//添加结点
                AddTreeNode(root);
                break;
            case '0':
                break;
            default:
                ;
        }
    }while(menusel!='0');

    //遍历
    do{
        printf("请选择菜单遍历二叉树,输入0表示退出:\n");
        printf("1.先序遍历DLR\t");			//显示菜单
        printf("2.中序遍历LDR\n");
        printf("3.后序遍历LRD\t");
        printf("4.按层遍历\n");
        menusel=getch();
        switch(menusel)
        {
        case '0':
             break;
        case '1':							//先序遍历
             printf("\n先序遍历DLR的结果：");
             DLRTree(root,TreeNodeData1);
             printf("\n");
             break;
        case '2':							//中序遍历 
             printf("\n中序LDR遍历的结果：");
             LDRTree(root,TreeNodeData1);
             printf("\n");
             break;
        case '3':							//后序遍历 
             printf("\n后序遍历LRD的结果：");
             LRDTree(root,TreeNodeData1);
             printf("\n");
             break;
        case '4':							//按层遍历
             printf("\n按层遍历的结果：");
             LevelTree(root,TreeNodeData1);
             printf("\n");
             break;
        default:
            ;
        }
    }while(menusel!='0');
    //深度
    printf("\n二叉树深度为:%d\n",TreeDepth(root));

    ClearTree(root);						//清空二叉树 
    root=NULL;
}
