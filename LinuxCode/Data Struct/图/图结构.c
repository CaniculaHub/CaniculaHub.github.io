准备数据
可以在程序中准备相应的数据用于保存图结构。程序示例代码如下：
#define MaxNum 20   					//图的最大顶点数   
#define MaxValue 65535 					//最大值（可设为一个最大整数）
typedef struct
{
    char Vertex[MaxNum]; 				//保存顶点信息（序号或字母）
    int GType;							//图的类型（0:无向图，1:有向图）
    int VertexNum; 						//顶点的数量 
    int EdgeNum;						//边的数量 
    int EdgeWeight[MaxNum][MaxNum];		//保存边的权 
    int isTrav[MaxNum]; 				//遍历标志 
}GraphMatrix; 							//定义邻接矩阵图结构
2.8.4  创建图
在使用图结构之前，首先要创建并初始化一个图。其程序示例代码如下：
void CreateGraph(GraphMatrix *GM)				//创建邻接矩阵图 
{
    int i,j,k;
    int weight;									//权
    char EstartV,EendV; 						//边的起始顶点 

    printf("输入图中各顶点信息\n");
    for(i=0;i<GM->VertexNum;i++) 				//输入顶点 
    {
        getchar();
        printf("第%d个顶点:",i+1);
        scanf("%c",&(GM->Vertex[i])); 			//保存到各顶点数组元素中 
    }
    printf("输入构成各边的顶点及权值:\n"); 
    for(k=0;k<GM->EdgeNum;k++)  				//输入边的信息 
    {
        getchar();
        printf("第%d条边：",k+1);
        scanf("%c %c %d",&EstartV,&EendV,&weight);
        for(i=0;EstartV!=GM->Vertex[i];i++);	//在已有顶点中查找始点 
        for(j=0;EendV!=GM->Vertex[j];j++); 		//在已有顶点中查找终点 
        GM->EdgeWeight[i][j]=weight; 			//对应位置保存权值，表示有一条边
        if(GM->GType= =0)  						//若是无向图
        {
            GM->EdgeWeight[j][i]=weight;			//在对角位置保存权值  
        }
    }
}
2.8.5  清空图
清空图的示例代码如下：
void ClearGraph(GraphMatrix *GM)
{
    int i,j;

    for(i=0;i<GM->VertexNum;i++)  				//清空矩阵 
    {
        for(j=0;j<GM->VertexNum;j++)
        {
            GM->EdgeWeight[i][j]=MaxValue;		//设置矩阵中各元素的值为MaxValue
        }
    }
}

2.8.6  显示图
显示图的示例代码如下：
void OutGraph(GraphMatrix *GM)						//输出邻接矩阵 
{
    int i,j;
    for(j=0;j<GM->VertexNum;j++)
    {
        printf("\t%c",GM->Vertex[j]);          		//在第1行输出顶点信息 
    }
    printf("\n");
    for(i=0;i<GM->VertexNum;i++) 
    {
        printf("%c",GM->Vertex[i]);
        for(j=0;j<GM->VertexNum;j++)
        {
            if(GM->EdgeWeight[i][j]= =MaxValue) 		//若权值为最大值 
            {
                printf("\tZ");          				//用Z表示无穷大
            }
            else
            {
                printf("\t%d",GM->EdgeWeight[i][j]); 	//输出边的权值
            }
        }
        printf("\n");
    }             
}
2.8.7  遍历图
深度优先遍历算法是一个递归过程，示例代码如下：
void DeepTraOne(GraphMatrix *GM,int n) 	//从第n个结点开始，深度遍历图 
{
    int i;
    GM->isTrav[n]=1; 						//标记该顶点已处理过 
    printf("->%c",GM->Vertex[n]);			//输出结点数据 

    //添加处理结点的操作 
    for(i=0;i<GM->VertexNum;i++)
    {
        if(GM->EdgeWeight[n][i]!=MaxValue && !GM->isTrav[n])
        {
            DeepTraOne(GM,i); 				//递归进行遍历 
        }
    }
}
void DeepTraGraph(GraphMatrix *GM) 		//深度优先遍历 
{
    int i;

    for(i=0;i<GM->VertexNum;i++) 			//清除各顶点遍历标志 
    {
        GM->isTrav[i]=0;
    }
    printf("深度优先遍历结点:"); 
    for(i=0;i<GM->VertexNum;i++)
    {
        if(!GM->isTrav[i]) 					//若该点未遍历 
        {
            DeepTraOne(GM,i); 				//调用函数遍历
        }
    }
    printf("\n"); 
}
