#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
void * thread()
{
	printf("thread wait...\n");
	sleep(2);
	return NULL;
}
 int main(int argc, const char *argv[])
 {
 	pthread_t ID;
	pthread_create(&ID,NULL,thread,NULL);
	pthread_join(ID,NULL);
	printf("mian GOING\n");
 	return 0;
 }
