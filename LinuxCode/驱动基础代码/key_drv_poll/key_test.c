#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>


struct key_event{
	int code; // ��ʾ����������:  home, esc, Q,W,E,R,T, ENTER
	int value; // ��ʾ���»���̧�� 1 / 0
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

	//��ض���ļ�fd
	struct pollfd pfd[2];

	pfd[0].fd = fd; //��ذ����豸
	pfd[0].events = POLLIN;

	pfd[1].fd = 0; //��׼����
	pfd[1].events = POLLIN;
	
	while(1)
	{
			ret = poll(pfd, 2, -1); // ������Ҫȥʵ��poll�ӿڣ�������open�� read��
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
