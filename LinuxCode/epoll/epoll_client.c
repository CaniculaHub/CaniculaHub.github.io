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
#include <pthread.h>
#define SERV_PORT 8000
#define BACKLOG 5

#define EXIT "exit"

int main(int argc, const char *argv[])
{

	int clientfd;
	struct sockaddr_in clientaddr;
	if(argc<2)
	{
		printf("uasage: input your IP<\n");
		exit(-1);
	}
	if ((clientfd = socket (AF_INET, SOCK_STREAM, 0)) < 0) 
	{
		perror ("socket");
		exit (1);
	}
	
	bzero (&clientaddr, sizeof (clientaddr));
	clientaddr.sin_family = AF_INET;
	clientaddr.sin_port = htons (SERV_PORT);	

	if (inet_pton (AF_INET, argv[1], (void *) &clientaddr.sin_addr) != 1) 
	{
		perror ("inet_pton");
		exit (1);
	}


	if (connect (clientfd, (struct sockaddr *) &clientaddr, sizeof (clientaddr)) < 0) 
	{
		perror ("connect");
		exit (1);
	}

	printf ("Client staring...OK!\n");

	char buf[BUFSIZ];
	int ret = -1;
	while (1) 
	{
		bzero (buf, BUFSIZ);
		if (fgets (buf, BUFSIZ - 1, stdin) == NULL) 
		{
			continue;
		}
		do 
		{
			ret = write (clientfd, buf, strlen (buf));
		} while (ret < 0 && EINTR == errno);

		if (!strncasecmp (buf, EXIT, strlen (EXIT))) 
		{	
			printf ("Client is exiting!\n");
			break;
		}
	}
	close (clientfd);
}
