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
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SERV_PORT 8000
#define BACKLOG 5

#define EXIT  "exit"
#define led_on "ON"
#define led_off "OFF"

int main(int argc, const char *argv[])
{
	
	int sockfd ,connectfd;
	struct sockaddr_in serveraddr,clientaddr;

	if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) < 0) 
	{
		perror ("socket");
		exit (1);
	}

	/*填充struct sockaddr_in结构体变量 */
	bzero (&serveraddr, sizeof (serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons (SERV_PORT);	
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind (sockfd, (struct sockaddr *) &serveraddr, sizeof (serveraddr)) < 0) 
	{
		perror ("bind");
		exit (1);
	}

	if (listen (sockfd, BACKLOG) < 0) 
	{
		perror ("listen");
		exit (1);
	}
	printf ("Server starting....OK!\n");

	socklen_t addrlen = sizeof (clientaddr);
	if ((connectfd = accept (sockfd, (struct sockaddr *) &clientaddr, &addrlen)) < 0) 
	{
		perror ("accept");
		exit (1);
	}

	char ipv4_addr[16];
	if (!inet_ntop (AF_INET, (void *) &clientaddr.sin_addr, ipv4_addr, sizeof (clientaddr))) 
	{
		perror ("inet_ntop");
		exit (1);
	}

	printf ("Clinet(%s:%d) is connected!\n", ipv4_addr, ntohs (clientaddr.sin_port));

	int ret = -1;
	char buf[BUFSIZ];
	int led_fd;
	int led_flag;
	led_fd = open("/dev/led0", O_RDWR);
	if(led_fd < 0)
	{
		perror("open error");
		exit(1);
	}
	while (1) 
	{
		bzero (buf, BUFSIZ);
		do 
		{
			ret = read (connectfd, buf, BUFSIZ - 1);
		} while (ret < 0 && EINTR == errno);
		if (ret < 0) 
		{
			perror ("read");
			exit (1);
		}
		if (!ret) 
		{				//对方已经关闭
			break;
		}
		printf ("Receive data: %s\n", buf);
		printf ("please input ON or OFF to control LED");
		if (!strncasecmp (buf, led_on, strlen (led_on)))
		{	
			led_flag=1;
			write(led_fd,&led_flag, 4);
			printf ("LED IS OPEN!\n");
		}
		if (!strncasecmp (buf, led_off, strlen (led_off)))
		{	
			led_flag=0;
			write(led_fd, &led_flag, 4);
			printf ("LED IS off!\n");
		}
		if (!strncasecmp (buf, EXIT, strlen (EXIT)))
		{	
			printf ("Client is exiting!\n");
			break;
		}
	}
	close (connectfd);
	close (sockfd);
	return 0;
}
