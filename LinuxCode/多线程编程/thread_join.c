#include <stdio.h>
#include <pthread.h>
void *thread(void *str)
{
	printf("thread is created!\n");
	pthread_exit((void *)1);//这里的参数是返回值任意值
}
 int main(int argc, const char *argv[])
 {
 	pthread_t ID;
	int res;
	void * temp;
	res=pthread_create(&ID,NULL,thread,NULL);
	printf("this is the main thread\n");
	if (res)
	{
		printf("thread is not created!\n");
		return -1;
	}
	res=pthread_join(ID,&temp);//将标记值传给函数
	if (res)
	{
		printf("thread is not exit!\n");
		return -2;
	}
	printf("thread code is %d\n",(int)temp);
 	return 0;
 }
