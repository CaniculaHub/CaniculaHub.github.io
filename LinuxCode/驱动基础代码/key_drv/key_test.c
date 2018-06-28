#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


struct key_event{
	int code; // ��ʾ����������:  home, esc, Q,W,E,R,T, ENTER
	int value; // ��ʾ���»���̧�� 1 / 0
};

#define KEY_ENTER		28

int main(int argc, char *argv[])
{
	struct key_event event;

	int fd = open("/dev/key0", O_RDWR);
	if(fd < 0)
	{
		perror("open");
		exit(1);
	}

	while(1)
	{
		read(fd, &event, sizeof(struct key_event));

		if(event.code == KEY_ENTER)
		{
			if(event.value)
			{
				printf("APP__ key enter pressed\n");
			}else{
				printf("APP__ key enter up\n");
			}
		}
	}


	close(fd);


	return 0;

}


