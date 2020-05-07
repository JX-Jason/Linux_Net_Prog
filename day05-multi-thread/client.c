#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
// #include <
#define BUF_SIZE 100
#define NAME_SIZE 20

char name[NAME_SIZE] = "[Default]";
char msg[BUF_SIZE];
void error_handling(char *message)
{
    perror(message);
    exit(1);

}
void *send_msg(void *arg)
{
    int fd = *((int *)arg);
    char name_msg[NAME_SIZE + BUF_SIZE];

    while (1)
    {
        fgets(msg, BUF_SIZE, stdin);
        if(!strcmp(msg, "q\n") || !strcmp(msg, "Q\n"))
        {
            close(fd);
            exit(0);
        }
        sprintf(name_msg, "%s %s", name, msg);
        write(fd, name_msg, strlen(name_msg));
    }
    return NULL;
}
void *recv_msg(void *arg)
{
    int fd = *((int *)arg);
    char name_msg[NAME_SIZE + BUF_SIZE];
    int str_len;
    while (1)
    {
        str_len = read(fd, name_msg, NAME_SIZE + BUF_SIZE - 1);
        if(str_len == -1)
        {
            return (void *)-1;
        }
        name_msg[str_len] = 0;
        fputs(name_msg, stdout);
    }
    return NULL;
}
int main(int argc, char **argv)
{
    struct sockaddr_in serv_addr;
    int fd;
    pthread_t send_thread, recv_thread;
    void *thread_return;

    if(argc != 3)
    {
        printf("Usage:%s <Port> <name>",argv[0]);
        exit(1);
    }

    sprintf(name, "[%s]", argv[2]);//格式化字符串为[wzj]的格式；

    fd = socket(PF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[1]));
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(connect(fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect error");
    
    pthread_create(&send_thread, NULL, send_msg, (void *)&fd);
    pthread_create(&recv_thread, NULL, recv_msg, (void *)&fd);

    pthread_join(send_thread, &thread_return);
    pthread_join(recv_thread, &thread_return);

    close(fd);
    return 0;
}