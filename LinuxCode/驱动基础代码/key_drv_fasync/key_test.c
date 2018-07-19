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
	int code; // ��ʾ����������:  home, esc, Q,W,E,R,T, ENTER
	int value; // ��ʾ���»���̧�� 1 / 0
};

#define KEY_ENTER		28

static int fd;
static struct key_event event;

 void catch_signale(int signo)
{
	if(signo == SIGIO)
	{
		printf("we got sigal SIGIO\n");
		// ��ȡ����
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

	// 1,�����źŴ�����
	signal(SIGIO,catch_signale);
	// 2������ǰ�������ó�SIGIO����������
	fcntl(fd, F_SETOWN, getpid());

	// 3����ioģʽ���ó��첽ģʽ
	int flags  = fcntl(fd, F_GETFL);
	fcntl(fd, F_SETFL, flags | FASYNC );


	while(1)
	{
		// ����������������
		printf("I am waiting......\n");
		sleep(1);
	}

	close(fd);


	return 0;

}


