#include <stdio.h>
#include <string.h>//memset
#include <sys/socket.h>//socklen_t
#include <arpa/inet.h>//sockaddr_in
#include <stdlib.h>

void error_handling(char *message)
{
    fputs(message,stderr);//fputs dont include void character,so use fputc to out \n to stderr stream.
    fputc('\n',stderr);
    exit(1);
}

int main(int argc,char **argv)
{
    int servfd,clntfd;
    struct sockaddr_in serv_addr;//the structure to describe IP address 127.0.0.1
    struct sockaddr_in clnt_addr;
    socklen_t          clnt_addr_size;

    char message[] = "Hello My First Server Program~";

    if(argc != 2)
    {
        printf("Usage : %s <port>\n",argv[0]);
        exit(1);
    }
    //01:create a socket,return a fd
    servfd = socket(PF_INET,SOCK_STREAM,0);// SOCK_STREAM:TCP/ Sequenced, reliable, connection-based byte streams.
    if(servfd == -1)
    {
        error_handling("socket error!");
    }
    //02:set 0
    memset(&serv_addr,0,sizeof(serv_addr));//let serv_addr all be zero
    //03:set IPV4/6 --> (addr|port)---NET-->
    serv_addr.sin_family = AF_INET;//IPV4
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);//host to net long:Address to accept any incoming messages
    serv_addr.sin_port = htons(atoi(argv[1]));//host to net short : port number

    //04:bind
    if(bind(servfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) == -1)
    {
        error_handling("bind error!");
    }

    //05:listen
    if(listen(servfd,5) == -1)
    {
        error_handling("listen error!");
    }

    //06:connect
    clnt_addr_size = sizeof(clnt_addr);
    clntfd = accept(servfd,(struct sockaddr*)&clnt_addr,&clnt_addr_size);
    if(clntfd == -1)
    {
        error_handling("accept error!");
    }

    write(clntfd,message,sizeof(message));
    close(clntfd);
    close(servfd);
    return 0;
}