#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
 #include <signal.h>

struct key_event{
	int code; // 表示按键的类型:  home, esc, Q,W,E,R,T, ENTER
	int value; // 表示按下还是抬起 1 / 0
};

#define KEY_ENTER		28

static int fd;
static struct key_event event;

 void catch_signale(int signo)
{
	if(signo == SIGIO)
	{
		printf("we got sigal SIGIO\n");
		// 读取数据
		read(fd, &event, sizeof(struct key_event));
		if(event.code == KEY_ENTER)
		{
			if(event.value)
			{
				printf("APP__ key enter pressed\n");
			}else
			{
				printf("APP__ key enter up\n");
			}
		}
	}

}

int main(int argc, char *argv[])
{
	int ret;
	
	fd = open("/dev/key0", O_RDWR);
	if(fd < 0)
	{
		perror("open");
		exit(1);
	}

	// 1,设置信号处理方法
	signal(SIGIO,catch_signale);
	// 2，将当前进程设置成SIGIO的属主进程
	fcntl(fd, F_SETOWN, getpid());

	// 3，将io模式设置成异步模式
	int flags  = fcntl(fd, F_GETFL);
	fcntl(fd, F_SETFL, flags | FASYNC );


	while(1)
	{
		// 可以做其他的事情
		printf("I am waiting......\n");
		sleep(1);
	}

	close(fd);


	return 0;

}


