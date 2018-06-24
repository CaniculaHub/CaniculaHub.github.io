#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX 50
int R[MAX];
void Bubble_sort(int N)
{
    int i,j;
    bool flag = true;//设置标记
    int temp; //将数组的数存入变量当中
    for ( i=0; i<N-1&&flag; i++)//i表示插入次数，共进行n-1次比较
    {
        for(j=0;j<N-i-1;j++)
        {
            flag = false;
            if(R[j]>R[j+1])
            {
                temp=R[j];
                R[j]=R[j+1];
                R[j+1]=temp;
                flag = true;
            }
        }
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
    Bubble_sort(n);
    printf("********************************\n ");
    for(i=0;i<n;i++)
    {
        printf("%d ",R[i]);
    }
    fflush(stdin);
    getchar();
}
