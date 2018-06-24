#include <stdio.h>
#include <stdlib.h>
#define MAX 50
int R[MAX];
/****************************************************
将数组的数存入变量当中
i表示插入次数，共进行n-1次插入
把待排序元素赋给temp，temp在while循环中并不改变，这样方便比较，并且它是要插入的元素
while循环的作用是将比当前元素大的元素都往后移动一个位置
顺序比较和移动,依次将元素后移动一个位置
元素后移后要插入的位置就空出了，找到该位置插入
*****************************************************/
void insert_sort(int N)
{
    int i,j;
    int temp;
    for ( i=1; i<N; i++)
    {
        temp=R[i];
        j=i-1;    
        while ((j>=0)&& (temp<R[j]))
        {
            R[j+1]=R[j];
            j--; 
        }
        R[j+1]=temp;                        
    }
}
 void main()
 {
     int i,n;
     printf("input your cyele number but less than 50\n");
     scanf("%d",&n);
     if(n<0||n>50)
     {
         printf("error");
         exit(0);
     }
     printf("please input the elenment one by one\n");
     for(i=0;i<n;i++)
     {
         scanf("%d",&R[i]);
         printf("your input number is %d\n",R[i]);
     }
     insert_sort(n);
     printf("********************************\n ");
     for(i=0;i<n;i++)
     {
         printf("%d ",R[i]);
     }
     fflush(stdin);
     getchar();
 }