#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#define BUF_SIZE 30
void error_handling(char *message)
{
    perror(message);
    exit(1);
}
int main(int argc,char **argv[])
{
    fd_set reads,cpy_reads;
    int result,str_len;
    char buf[BUF_SIZE];
    struct timeval timeout;
    int servfd,clntfd;
    struct sockaddr_in servaddr,clntaddr;//IPV4 addr structure

    int fd_max,fd_num;

    if(argc != 2)
    {
        printf("Usage: %s <Port>\n",argv[0]);
    }

    servfd = socket(AF_INET,SOCK_STREAM,0);
    if(servfd == -1)
    {
        error_handling("socket error");
    }

    memset(&servaddr,0,sizeof(&servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(atoi(argv[1]));

    if(bind(servfd,(struct sockaddr *)&servaddr,sizeof(servaddr)))
    {
        error_handling("bind error");
    }

    if(listen(servfd,5) == -1)
        error_handling("listen error");

    FD_ZERO(&reads);
    FD_SET(servfd,&reads);
    fd_max = servfd;
    while(1)
    {
        cpy_reads = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec=5000;

        if((fd_num = select(fd_max+1,&cpy_reads,0,0,&timeout)) == -1)
            break;

        if(fd_num == 0)
            continue;

        for(int i = 0;i<fd_max+1;i++)
        {
            if(FD_ISSET(i,&cpy_reads))
            {
                if(i == servfd)
                {
                    socklen_t clntsocklen = sizeof(clntaddr);
                    clntfd = accept(i,(struct sockaddr*)&servaddr,&clntsocklen);
                    //clntfd = accept(i,(struct sockaddr*)&servaddr,sizeof(clntaddr));
                    FD_SET(clntfd,&reads);//reset 1
                    if(fd_max < clntfd)
                        fd_max = clntfd;//last fd occur
                    printf("connected! client :%d\n",clntfd);
                }
                else
                {
                    str_len = read(i,buf,BUF_SIZE);
                    if(str_len == 0)
                    {
                        FD_CLR(i,&reads);
                        close(i);
                        printf("closed client:%d \n",i);
                    }
                    else
                    {
                        printf("server accept:%s \n",buf);
                        write(i,buf,BUF_SIZE);
                    }                   
                    
                }
                
            }

        }
    }

    close(servfd);
    return 0;
}