#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
void* thread_main(void *arg)
{
    int i;
    int cnt = *((int *)arg);
    char *msg = (char *)malloc(sizeof(char) * 50);
    strcpy(msg,"Hello,I'm a thread~ \n");
    for (i = 0; i < cnt; i++)
    {
        sleep(1);
        puts("running thread");
    }
    return (void*)msg;    
}
int main(int argc, char **argv)
{
    pthread_t t_id;
    int thread_params = 5;
    void *thread_return;
    if(pthread_create(&t_id,NULL, thread_main, (void*)&thread_params) != 0)
    {
        puts("pthread_create error");
        return -1;
    }

    if(pthread_join(t_id,&thread_return) != 0)
    {
        puts("pthread_join error");
        return -1;
    }

    printf("thread return message: %s \n", (char *)thread_return);
    free(thread_return);
    puts("end of main");
    return 0;
}