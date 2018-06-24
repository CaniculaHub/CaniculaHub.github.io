void InsertionSort(int *a,int len)  		//从大到小的插入排序
{
    int i,j,t,h;

    for (i=1;i<len;i++)						
    {
        t=a[i];
        j=i-1;
        while(j>=0 && t>a[j])//从大到小的顺序
        {
            a[j+1]=a[j];
            j--;
        }
        a[j+1]=t;
        printf("第%d步排序结果:",i);			
        for(h=0;h<len;h++)
        {
            printf("%d ",a[h]);			
        }
        printf("\n");
    }
}
