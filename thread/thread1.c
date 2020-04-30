#include <stdio.h>
#include <pthread.h>
void* thread_main(void *arg)
{
    int i;
}
int main(int argc, char **argv)
{
    pthread_t t_id;
    int thread_params = 5;
    if(pthread_create(&t_id,NULL, thread_main,))
}