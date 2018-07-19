#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>


struct key_event{
	int code; // 表示按键的类型:  home, esc, Q,W,E,R,T, ENTER
	int value; // 表示按下还是抬起 1 / 0
};

#define KEY_ENTER		28

int main(int argc, char *argv[])
{
	int ret;
	struct key_event event;
	char in_buf[128];

	int fd = open("/dev/key0", O_RDWR);
	if(fd < 0)
	{
		perror("open");
		exit(1);
	}

	//监控多个文件fd
	struct pollfd pfd[2];

	pfd[0].fd = fd; //监控按键设备
	pfd[0].events = POLLIN;

	pfd[1].fd = 0; //标准输入
	pfd[1].events = POLLIN;
	
	while(1)
	{
			ret = poll(pfd, 2, -1); // 驱动需要去实现poll接口，类似于open， read，
			printf("ret = %d\n", ret);
			
			if(ret > 0)
			{
				if(pfd[0].revents & POLLIN)
				{
					read(pfd[0].fd, &event, sizeof(struct key_event));
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
				if(pfd[1].revents & POLLIN)
				{
					fgets(in_buf, 128, stdin);
					printf("in_buf = %s\n", in_buf);
				}
			}else{
				perror("poll");
				exit(1);
			}
	}



	close(pfd[0].fd);


	return 0;

}
