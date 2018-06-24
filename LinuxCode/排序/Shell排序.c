void ShellSort(int *a,int len) //Shell排序
{
    int i,j,h;
    int r,temp;
    int x=0;

    for(r=len/2;r>=1;r/= 2)						
    {
     for(i=r;i<len;i++)
     {
            temp=a[i];
            j=i-r;
            while(j>=0 && temp<a[j])
            {
                a[j+r]=a[j];
                j-=r;
            }
            a[j+r]=temp;
        }

        x++;
        printf("第%d步排序结果:",x);				
        for(h=0;h<len;h++)
        {
            printf("%d ",a[h]);					
        }
        printf("\n");
    }
}
