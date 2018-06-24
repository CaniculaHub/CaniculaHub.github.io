#ifndef __MAKEU_NET_H__
#define __MAKEU_NET_H__

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h>			/* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>			/* superset of previous */

#define SERV_PORT 5001
#define SERV_IP_ADDR "192.168.7.246"
#define BACKLOG 5

#define QUIT_STR "quit"

#endif


int main (void)
{
	int fd = -1;
	struct sockaddr_in sin;
	/* 1. 创建socket fd */
	if ((fd = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
		perror ("socket");
		exit (1);
	}
	/*2.连接服务器 */

	/*2.1 填充struct sockaddr_in结构体变量 */
	bzero (&sin, sizeof (sin));

	sin.sin_family = AF_INET;
	sin.sin_port = htons (SERV_PORT);	//网络字节序的端口号
#if 0
	sin.sin_addr.s_addr = inet_addr (SERV_IP_ADDR);
#else
	if (inet_pton (AF_INET, SERV_IP_ADDR, (void *) &sin.sin_addr) != 1) {
		perror ("inet_pton");
		exit (1);
	}
#endif

	if (connect (fd, (struct sockaddr *) &sin, sizeof (sin)) < 0) {
		perror ("connect");
		exit (1);
	}

	printf ("Client staring...OK!\n");
	/*3. 读写数据 */
	char buf[BUFSIZ];
	int ret = -1;
	while (1) {
		bzero (buf, BUFSIZ);
		if (fgets (buf, BUFSIZ - 1, stdin) == NULL) {
			continue;
		}
		do {
			ret = write (fd, buf, strlen (buf));
		} while (ret < 0 && EINTR == errno);

		if (!strncasecmp (buf, QUIT_STR, strlen (QUIT_STR))) {	//用户输入了quit字符
			printf ("Client is exiting!\n");
			break;
		}
	}

	/*4.关闭套接字 */
	close (fd);
}
