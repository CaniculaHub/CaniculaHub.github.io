/*thread_create.c*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> 
#include <unistd.h>  
void thread_one(void)
{
	int i;
	for(i=0;i<5;i++)
	{
		printf("one one thread\n");
		sleep(2);
	}
}
void thread_two(void)
{
	int i;
	for(i=0;i<5;i++)
	{
		printf("two two thread\n");
		sleep(2);
	}
}

int main(int argc, const char *argv[])
{
	pthread_t ID_one,ID_two;
	int flag; //用来表示创建是否成功 0成功 非0失败
	/*创建一个线程，并使得线程执行thread_one函数*/
	flag=pthread_create(&ID_one,NULL,(void *)thread_one,NULL);
	if(flag)
	{
		printf("pthread_one error\n");
		return 1;
	}
	/*创建一个线程，并使得线程执行thread_two函数*/
	flag=pthread_create(&ID_two,NULL,(void *)thread_two,NULL);
	if(flag)
	{
		printf("pthread_two error\n");
		return 1;
	}

	pthread_join(ID_one,NULL);
	pthread_join(ID_two,NULL);
	return 1;
}
