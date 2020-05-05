#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>//unistd.h 是 C 和 C++ 程序设计语言中提供对 POSIX 操作系统 API 的访问功能的头文件的名称。
#include <string.h>
#include <sys/types.h>//sys/types.h，中文名称为基本系统数据类型，此头文件还包含适当时应使用的多个基本派生类型。
#include <sys/socket.h>//
#include <arpa/inet.h>//在linux下网络编程程中常用头文件，主要是信息转换，将客户端信息，转换为字符串信息。
#include <dirent.h>//是POSIX.1标准定义的unix类目录操作的头文件，包含了许多UNIX系统服务的函数原型，例如opendir函数、readdir函数.
#include <fcntl.h>//用来避免一些系统安全
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h> 
#define IPADDRESS   "127.0.0.1"
#define PORT        9000
#define MAXSIZE     256
#define LISTENQ     5
#define FDSIZE      1000
#define EPOLLEVENTS 100


void commd_ls(int);
void commd_get(int, char *);
void commd_put(int, char *);
void login(int cli_sockfd);
char *username="yan";//指定的用户名密码
char *pwd="123456";
int login_flg=0;
//函数声明
//创建套接字并进行绑定
static int socket_bind(const char* ip,int port);
//IO多路复用epoll
static void do_epoll(int listenfd);
//事件处理函数
static void
handle_events(int epollfd,struct epoll_event *events,int num,int listenfd,char *buf);
//处理接收到的连接
static void handle_accpet(int epollfd,int listenfd);
//读处理
static int do_read(int epollfd,int fd,char *buf,int n);
//写处理
static int do_write(int epollfd,int fd,char *buf,int n);
//添加事件
static void add_event(int epollfd,int fd,int state);
//修改事件
static void modify_event(int epollfd,int fd,int state);
//删除事件
static void delete_event(int epollfd,int fd,int state);
 
int main(int argc,char *argv[])
{
    int  listenfd;
    listenfd = socket_bind(IPADDRESS,PORT);
    listen(listenfd,LISTENQ);
    do_epoll(listenfd);
    return 0;
}
 
static int socket_bind(const char* ip,int port)
{
    int  listenfd;
    struct sockaddr_in servaddr;
    listenfd = socket(AF_INET,SOCK_STREAM,0);
    if (listenfd == -1)
    {
        perror("socket error:");
        exit(1);
    }
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET,ip,&servaddr.sin_addr);
    servaddr.sin_port = htons(port);
    if (bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr)) == -1)
    {
        perror("bind error: ");
        exit(1);
    }
    return listenfd;
}
 
static void do_epoll(int listenfd)
{
    int epollfd;
    struct epoll_event events[EPOLLEVENTS];
    int ret;
    char buf[MAXSIZE];
    memset(buf,0,MAXSIZE);
    //创建一个描述符
    epollfd = epoll_create(FDSIZE);
    //添加监听描述符事件
    add_event(epollfd,listenfd,EPOLLIN);//对应的文件描述符可以读时通知
    for ( ; ; )
    {
        //获取已经准备好的描述符事件
        ret = epoll_wait(epollfd,events,EPOLLEVENTS,-1);//可读时通知，读到后就改为写时通知
        handle_events(epollfd,events,ret,listenfd,buf);//处理已准备好的事件
    }
    close(epollfd);
}
 
static void
//逻辑有问题
handle_events(int epollfd,struct epoll_event *events,int num,int listenfd,char *buf)
{
    int i;
    int fd;
    //进行选好遍历
    for (i = 0;i < num;i++)
    {
        fd = events[i].data.fd;
        //根据描述符的类型和事件类型进行处理
        if ((fd == listenfd) &&(events[i].events & EPOLLIN))//监听套接字
            handle_accpet(epollfd,listenfd);
        else if (events[i].events & EPOLLIN)//客户套接字可读
        {
 
            do_read(epollfd,fd,buf,MAXSIZE);
 
             if(strncmp(buf,"ls",2) == 0)
        {
            commd_ls(fd);
        }else if(strncmp(buf,"get", 3) == 0 )
        {
            commd_get(fd, buf+4);
        }else if(strncmp(buf, "put", 3) == 0)
        {
            commd_put(fd, buf+4);
        }else if(strncmp(buf, "login",5) == 0){
            login(fd);
        }else
        {
            printf("Error!Command Error!\n");
        }
        }
        else if (events[i].events & EPOLLOUT)//客户套接字可写
            do_write(epollfd,fd,buf,strlen(buf));
    }
}
static void handle_accpet(int epollfd,int listenfd)
{
    int clifd;
    struct sockaddr_in cliaddr;
    socklen_t  cliaddrlen;
    clifd = accept(listenfd,(struct sockaddr*)&cliaddr,&cliaddrlen);
    if (clifd == -1)
        perror("accpet error:");
    else
    {
        printf("accept a new client: %s:%d\n",inet_ntoa(cliaddr.sin_addr),cliaddr.sin_port);
        //添加一个客户描述符和事件
        add_event(epollfd,clifd,EPOLLIN);//可读时通知，读到后就改为写时通知
    }
}
 
static int do_read(int epollfd,int fd,char *buf,int n)
{
    int nread;
    nread = read(fd,buf,n);
    printf("%d\n",nread);
    if (nread == -1)
    {
        perror("read error:");
        close(fd);
        delete_event(epollfd,fd,EPOLLIN);
    }
    else if (nread == 0)
    {
        fprintf(stderr,"client close.\n");
        close(fd);
        delete_event(epollfd,fd,EPOLLIN);
    }
    else
    {
        printf("read message is : %s",buf);
        //修改描述符对应的事件，由读改为写
        modify_event(epollfd,fd,EPOLLOUT);//把当前描述符设为可写时通知
    }
    return nread;
}
 
static int do_write(int epollfd,int fd,char *buf,int n)
{
    int nwrite;
    nwrite = write(fd,buf,n);
    if (nwrite == -1)
    {
        perror("write error:");
        close(fd);
        delete_event(epollfd,fd,EPOLLOUT);
    }
    else
        modify_event(epollfd,fd,EPOLLIN);//把当前描述符设为可写时通知
    memset(buf,0,MAXSIZE);
    return nwrite;
}
 
static void add_event(int epollfd,int fd,int state)
{
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&ev);
}
 
static void delete_event(int epollfd,int fd,int state)
{
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,&ev);
}
 
static void modify_event(int epollfd,int fd,int state)
{
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd,EPOLL_CTL_MOD,fd,&ev);//EPOLL_CTL_MOD：修改已经注册的fd的监听事件；
}
 
 
 
void commd_ls(int sockfd)//列出目录下到文件
{
    DIR * mydir =NULL;//目录对象信息结构体
    struct dirent *myitem = NULL;
    char commd[MAXSIZE] ;
    bzero(commd, MAXSIZE);
    //opendir为目录操作函数，类似于open函数
    //mydir中存有相关目录的信息（有待学习）
    if((mydir=opendir(".")) == NULL)
    {
        printf("OpenDir Error!\n");
        exit(1);
    }
 
    while((myitem = readdir(mydir)) != NULL)
    {
        if(sprintf(commd, myitem->d_name, MAXSIZE) < 0)
        {
            printf("Sprintf Error!\n");
            exit(1);
        }
 
        if(write(sockfd, commd, MAXSIZE) < 0 )
        {
            printf("Write Error!\n");
            exit(1);
        }
    }
 
    closedir(mydir);
    close(sockfd);
 
    return ;
}
 
void commd_get(int sockfd, char *filename)//下载文件
{
    int fd, nbytes;
    char buffer[MAXSIZE];
    bzero(buffer, MAXSIZE);
 
    printf("get filename : [ %s ]\n",filename);
    if((fd=open(filename, O_RDONLY)) < 0)
    {
        printf("Open file Error!\n");
        buffer[0]='N';
        if(write(sockfd, buffer, MAXSIZE) <0)
        {
            printf("Write Error!At commd_get 1\n");
            exit(1);
        }
        return ;
    }
 
    buffer[0] = 'Y';    //此处标示出文件读取成功
    if(write(sockfd, buffer, MAXSIZE) <0)
    {
        printf("Write Error! At commd_get 2!\n");
        close(fd);
        exit(1);
    }
 
    while((nbytes=read(fd, buffer, MAXSIZE)) > 0)
    {
        if(write(sockfd, buffer, nbytes) < 0)
        {
            printf("Write Error! At commd_get 3!\n");
            close(fd);
            exit(1);
        }
    }
 
    close(fd);
    close(sockfd);
 
    return ;
}
 
void commd_put(int sockfd, char *filename)//上传文件
{
    int fd, nbytes;
    char buffer[MAXSIZE];
    bzero(buffer, MAXSIZE);
 
    printf("get filename : [ %s ]\n",filename);
    if((fd=open(filename, O_WRONLY|O_CREAT|O_TRUNC, 0644)) < 0)
    {
        printf("Open file Error!\n");
        return ;
    }
 
    while((nbytes=read(sockfd, buffer, MAXSIZE)) > 0)
    {
        if(write(fd, buffer, nbytes) < 0)
        {
            printf("Write Error! At commd_put 1!\n");
            close(fd);
            exit(1);
        }
    }
 
    close(fd);
    close(sockfd);
 
    return ;
}
 
void login(int cli_sockfd){//登录
   int nbytes;
   char login_state[MAXSIZE];
   char tmp_username[MAXSIZE];
   char tmp_pwd[MAXSIZE];
   char *p;
 
   write(cli_sockfd,"start",10);
 
   read(cli_sockfd,tmp_username,MAXSIZE);
 
 
   if(strncmp(tmp_username,username,strlen(username))==0){
        printf("user [ %s ] is loging\n",username);
        strcpy(login_state,"userYes");
        write(cli_sockfd,login_state,MAXSIZE);
        bzero(login_state,MAXSIZE);
         read(cli_sockfd,tmp_pwd,MAXSIZE);
        if(strncmp(tmp_pwd,pwd,strlen(pwd))==0){
            login_flg=1;
            strcpy(login_state,"success");
 
            write(cli_sockfd,login_state,MAXSIZE);
 
            write(cli_sockfd,0,0);
            printf("login success\n");
            return;
        }
   }else{
         strcpy(login_state,"failed");
   }
   write(cli_sockfd,login_state,MAXSIZE);
 
}