void SelectionSort(int *a,int len)			
{
    int i,j,k,h;
    int temp;									

    for (i=0;i<len-1;i++)
    {
        k=i;
        for (j=i+1;j<len;j++)
        {
            if (a[j]<a[k])
                k=j;
        }
        if(k!=i)									
        {
            temp=a[i];
            a[i]=a[k];
            a[k]=temp;
        }

        printf("第%d步排序结果:",i);			
        for(h=0;h<len;h++)
        {	
            printf("%d ",a[h]);					
        }
        printf("\n");
    }
}
