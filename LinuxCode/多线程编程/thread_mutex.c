#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#define thread_MAX 3
pthread_mutex_t mutex;

void *thread_function(void *arg)
{
	int i;
	pthread_mutex_lock(&mutex);
	int thread_num =(int*)arg;
	printf("thread %d is starting...\n",thread_num);
	for(i=0;i<thread_MAX;i++)
	{
		printf("thread %d is runing %d\n",thread_num,i);
		sleep(1);
	}
	printf("thread %d is end!\n",thread_num);
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}
int main(int argc, const char *argv[])
{
	pthread_t thread[thread_MAX];
	int num,res;
	void *thread_ret;//thread_join
	
	pthread_mutex_init(&mutex,NULL);
	
	for(num=0;num<thread_MAX;num++)
	{
		res=pthread_create(&thread[num],NULL,thread_function,(void *)(num));
		if(res)
		{
			printf("create thread %d is failed\n",num);
			exit(res);
		}
	}
		printf("create thread success\n waiting for thread to finish...\n");
	
	for(num=0;num<thread_MAX;num++)
	{
		res=pthread_join(thread[num],&thread_ret);
		if(!res)
		{
			printf("thread %d joined\n",num);
		}
		else
		{
				printf("thread %d join failed\n",num);
		}
	}
	pthread_mutex_destroy(&mutex);
	return 0;	
}
