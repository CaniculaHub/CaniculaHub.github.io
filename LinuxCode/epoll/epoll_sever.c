#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <sys/socket.h>  
#include <poll.h>  
#include <sys/epoll.h>  
#include <sys/time.h>  
#include <netinet/in.h>  
#include <sys/select.h> 


#define PORT 8000
#define MAX_EVENTS 1024
#define REVLEN 64  
void RecvData(int fd);
int main(int argc, char const *argv[])
{
    /* code */
    int sockfd;
    int epollfd;
    int connectfd;
    
    struct sockaddr_in server_addr,client_addr;
    struct epoll_event ready_List[MAX_EVENTS];
    
    /* Init socket*/
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0)
    {
        printf("socket error!\n");
        return -1;
    }
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    /*  Init bind   */
    if(bind(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr))<0)
    {
        printf("bind error!\n");
        return -1;
    }

    if (listen(sockfd,10)<0)
    {
        printf("listen error!\n");
        return -1;
    }

    /*  epoll Init  */
    epollfd = epoll_create(10);
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = sockfd;
    socklen_t len;

    if(epoll_ctl(epollfd,EPOLL_CTL_ADD,sockfd,&event)<0)
    {
        printf("epoll_ctl error!\n");
        return -1;
    }

    while(1)
    {
        int ready_num;
        int n=0;
		int time = 3000;
        struct epoll_event event_new;  //new connect EPOLL 
        ready_num = epoll_wait(epollfd,ready_List,MAX_EVENTS,time);
        if (ready_num<0)
        {
            printf("epoll_wait error!\n");
            return -1;
        }
        else
            if (ready_num == 0)
        {
            printf("wait...\n");
        }
        for(n=0;n<ready_num;n++)
        {
            
             /* connect client socket */
            if((ready_List[n].events & EPOLLIN)&&ready_List[n].data.fd==sockfd)       
            {
                char ipv4_addr[16];
                len = sizeof(struct sockaddr_in);
                bzero(&client_addr,len);
                connectfd = accept(sockfd,(struct sockaddr_in*)&client_addr,&len);
                if(connectfd<0)
                {
                    printf("accept error!\n");
                    exit(1);
                }
                else
				{
	        	 if (!inet_ntop (AF_INET, (void *) &client_addr.sin_addr, ipv4_addr, sizeof (client_addr))) 
            	 {
		        	 printf ("inet_ntop\n");
	      	  		 exit (1);
	       		 }
			    printf ("Clinet(%s:%d) is connected!\n", ipv4_addr, ntohs(client_addr.sin_port));
                event_new.data.fd = connectfd;  
                event_new.events =  EPOLLIN;  
                epoll_ctl(epollfd, EPOLL_CTL_ADD, connectfd, &event_new); 
                }
			}
            else 
                 RecvData(ready_List[n].data.fd); 
        }

    }
    close(epollfd);  
    close(sockfd);  
    return 0;
}

void RecvData(int fd)
{
    int flag ;
    char buf[REVLEN];
    flag = read(fd,buf,REVLEN);
    if(flag == -1)
    {
        perror("read error:");
        close(fd);
    }
    if (0==flag)  
    {
        close(fd);
        printf("Disconnect!\n");
    }
    else 
    {
        printf("read message is : %s\n",buf);
    }
}
