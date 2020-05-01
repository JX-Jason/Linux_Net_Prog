#include <stdio.h>
#include <pthread.h>
int sum = 0;
void* thread_summation(void *arg)
{
    int start = ((int *)arg)[0];
    int end = ((int *)arg)[1];
    while(start <= end)
    {
        sum+=start;
        start++;
    }
    return NULL;    
}
int main(int argc, char **argv)
{
    pthread_t t_id1,t_id2;
    int range1[] = {1,5};
    int range2[] = {6,10};
    int thread_params = 5;
    void *thread_return;
    if(pthread_create(&t_id1,NULL, thread_summation, (void*)range1) != 0)
    {
        puts("pthread1_create error");
        return -1;
    }
    if(pthread_create(&t_id2,NULL, thread_summation, (void*)range2) != 0)
    {
        puts("pthread2_create error");
        return -1;
    }
    pthread_join(t_id1,NULL);
    pthread_join(t_id2,NULL);
    printf("result: %d \n",sum);
    
    return 0;
}