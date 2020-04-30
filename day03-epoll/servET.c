#include <stdio.h>
#include <string.h>//memset
#include <unistd.h> //malloc
#include <sys/socket.h>//socklen_t
#include <arpa/inet.h>//sockaddr_in
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>

#define BUF_SIZE 4
#define EPOLL_SIZE 50

void error_handling(char *message)
{
    perror(message);
    exit(1);
}
void setnonblockingmode(int fd)
{
    int flag = fcntl(fd, F_GETFL, 0);
    fcntl(fd,F_SETFL, flag|O_NONBLOCK);
}
int main(int argc, char *argv[])
{
    int servfd,clntfd;
    struct sockaddr_in serv_addr,clnt_addr;
    socklen_t adr_size;
    int str_len,i;
    char buf[BUF_SIZE];

    struct epoll_event *ep_events;
    struct epoll_event event;
    int epfd,event_cnt;

    if(argc != 2)
    {
        printf("usage: %s <port>\n",argv[0]);
        exit(1);
    }

    servfd = socket(PF_INET,SOCK_STREAM,0);
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(servfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) == -1)
        error_handling("bind error");
    
    if(listen(servfd,5) == -1)
        error_handling("listen error");
    //create fd    
    epfd = epoll_create(EPOLL_SIZE);
    ep_events = malloc(sizeof(struct epoll_event) * EPOLL_SIZE);

    event.events = EPOLLIN;
    event.data.fd=servfd;
    epoll_ctl(epfd,EPOLL_CTL_ADD,servfd,&event);

    while(1)
    {
        event_cnt = epoll_wait(epfd,ep_events,EPOLL_SIZE,-1);
        if(event_cnt == -1)
        {
            puts("epoll_wait() error");
            break;
        }
        puts("return epoll_wait");
        for(int i = 0; i<event_cnt; i++)
        {
            if(ep_events[i].data.fd == servfd)//this fd occur something
            {
                adr_size = sizeof(clnt_addr);
                clntfd = accept(servfd,(struct sockaddr*)&clnt_addr,&adr_size);
                setnonblockingmode(clntfd);
                event.events = EPOLLIN|EPOLLET;
                event.data.fd=clntfd;
                epoll_ctl(epfd,EPOLL_CTL_ADD,clntfd,&event);
                printf("Connected client: %d \n",clntfd);
            }
            else
            {
                while(1)
                {
                    str_len = read(ep_events[i].data.fd, buf, BUF_SIZE);
                    if(str_len == 0)
                    {
                        epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
                        close(ep_events[i].data.fd);
                        printf("close client:%d \n",ep_events[i].data.fd);
                        break;
                    }
                    else if(str_len < 0)
                    {
                        if(errno == EAGAIN)
                            break;
                    }   
                    else
                    {
                        write(ep_events[i].data.fd, buf, str_len);
                    }
                                     
                }

                
            }
            
        }
    }

    close(servfd);
    close(epfd);
    return 0;
}