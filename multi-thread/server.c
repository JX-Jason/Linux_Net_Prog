#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>//memset
#include <stdlib.h>
#define BUF_SIZE 100
#define MAX_CLNT 256

int clnt_cnt = 0;
int clnt_fds[MAX_CLNT];
pthread_mutex_t mutex;

void error_handling(char *message)
{
    perror(message);
    exit(1);

}

void send_msg(char *msg, int len)
{
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < clnt_cnt; i++)
    {
        write(clnt_fds[i], msg, len);
    }
    pthread_mutex_unlock(&mutex);    
}
//每一个客户端开一个线程，通话结束后，将取消这个客户端，所以要将后面的fd往前面移动一下
void *handle_clnt(void *arg)
{
    int clntfd = *((int *)arg);
    int str_len = 0,i;
    char msg[BUF_SIZE];

    while ((str_len = read(clntfd, msg, sizeof(msg))) != 0)
    {
        send_msg(msg, str_len);
    }

    pthread_mutex_lock(&mutex);
    for ( i = 0; i < clnt_cnt; i++)
    {
        if(clntfd == clnt_fds[i])
        {
            while(i++ < clnt_cnt - 1)
                clnt_fds[i] = clnt_fds[i+1];
            break;
        }
    }

    clnt_cnt--;
    pthread_mutex_unlock(&mutex);
    close(clntfd);
    return NULL;
}

int main(int argc, char **argv)
{
    int servfd, clntfd;
    struct sockaddr_in serv_adr, clnt_adr;
    int clnt_adr_size;
    pthread_t t_id;

    if(argc != 2)
    {
        printf("Error! Usage: %s <Port>",argv[0]);
        exit(1);
    }

    pthread_mutex_init(&mutex, NULL);

    servfd = socket(PF_INET, SOCK_STREAM, 0);
    
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_port = htons(atoi(argv[1]));
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    int tst = bind(servfd, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
    if(tst == -1)
        error_handling("bind error");

    if(listen(servfd,5))
        error_handling("listen error");  

    while(1)
    {
        clnt_adr_size = sizeof(clnt_adr);
        clntfd = accept(servfd, (struct sockaddr *)&clnt_adr, &clnt_adr_size);
        pthread_mutex_lock(&mutex);
        clnt_fds[clnt_cnt++] = clntfd;
        pthread_mutex_unlock(&mutex);

        pthread_create(&t_id, NULL, handle_clnt, (void *)&clntfd);
        pthread_detach(t_id);

        printf("Connected client IP: %s \n", inet_ntoa(clnt_adr.sin_addr));
    }
    close(servfd);
    return 0;
    //pthread_mutex_destroy(&mutex);    因为这里是detach，如果破坏mutex，主线程退出之后其他子线程就用不了mutex了  
}