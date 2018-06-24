void MergeOne(int a[],int b[],int n,int len) 	//完成一遍合并的函数 
{
    int i,j,k,s,e;

    s=0;
    while(s+len<n)  
    {
        e=s+2*len-1;
        if(e>=n)//最后一段可能少于len个结点 
        {
            e=n-1;
        }//相邻有序段合并
        k=s;
        i=s;
        j=s+len;
        while(i<s+len && j<=e)//如果两个有序表都未结束时，循环比较 
        {
            if(a[i]<=a[j])//如果较小的元素复制到数组b中 
            {
               b[k++]=a[i++];
            }
            else
            {
                b[k++]=a[j++];
            }
        }
        while(i<s+len)//未合并的部分复制到数组b中 
        {
            b[k++]=a[i++];
        }
        while(j<=e)
        {
           b[k++]=a[j++]; //未合并的部分复制到数组b中 
        }

        s=e+1; 	//下一对有序段中左段的开始下标 
    }
    if(s<n) //将剩余的一个有序段从数组a中复制到数组b中
    {
        for(;s<n;s++)
        {
            b[s]=a[s];
        }
    }
}
void MergeSort(int a[],int n)//合并排序
{
    int *p;
    int h,count,len,f;

    count=0;//排序步骤
    len=1;  //有序序列的长度 
    f=0;	//变量f作标志
    if(!(p=(int *)malloc(sizeof(int)*n)))	//分配内存空间
    {
        printf("内存分配失败!\n");
        exit(0); 
    }
    while(len<n)
    {
        if(f= =1)//交替在a和P之间合并 
        {
            MergeOne(p,a,n,len);//p合并到a
        }
        else
        {
            MergeOne(a,p,n,len);//a合并到p
        }
        len=len*2;	//增加有序序列长度
        f=1-f; 	//使f值在0和1之间切换 

        count++;
        printf("第%d步排序结果:",count);//输出每步排序的结果
        for(h=0;h<SIZE;h++)
        {
            printf("%d ",a[h]);	//输出
        }
        printf("\n");

    }
    if(f)//如果进行了排序
    {
        for(h=0;h<n;h++)//将内存p中的数据复制到数组a中
        {
            a[h]=p[h];
        }
    }
    free(p); //释放内存 
}
