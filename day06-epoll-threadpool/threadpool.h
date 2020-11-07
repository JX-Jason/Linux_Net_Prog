#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <list>
#include <cstdio>
#include <exception>
#include <pthread.h>
#include <mutex>
#include <semaphore.h>
#include "locker.h"
template<typename T>
class threadpool
{
public:
    threadpool(int thread_number = 8, int max_request = 10000);
    ~threadpool();
    bool append(T* request);

private:
    static void* worker(void* arg);
    void run();
private:
    int m_thread_number;            /*线程池中的线程数*/
    int m_max_request;              /*请求队列中允许的最大请求数*/
    pthread_t* m_threads;           /*描述线程池的数组，大小为 m_thread_number*/
    std::list<T* > m_workqueue;     /*请求队列*/
    std::mutex  m_queuelocker;      /*保护请求队列的互斥锁*/
    sem m_queuestat;             /*是否有任务需要处理*/
    bool m_stop;                    /*是否结束线程*/
};

template<typename T>
threadpool<T>::threadpool(int thread_number, int max_request) :
                m_thread_number(thread_number), m_max_request(max_request), m_stop(false), m_threads(NULL)
{
    if((thread_number < 0) || (max_request < 0)){
        throw std::exception();
    }

    m_threads = new pthread_t[m_thread_number];

    if(!m_threads){
        throw std::exception();
    }

    /*创建thread_numer个线程，并将它们都设置为脱离线程*/
    for(int i = 0; i < thread_number; ++i){
        printf("create the %dth thread\n", i);
/*
    pthread_create()的第三个参数必须指向一个静态函数。而要在一个静态函数中使用类的动态成员（比如成员函数和成员变量，这些都是会变得嘛！）
    则只能通过以下两种方式调用：
    1、通过类的静态对象来调用。比如单例模式中，静态函数可以通过类的全局唯一实例来访问动态成员函数；
    2、将类的对象作为参数传递给该静态函数，然后在静态函数中引用这个对象，并调用其动态方法run。
*/
        if(pthread_create(m_threads + i, NULL, worker, this) != 0){
            delete[] m_threads;
            throw std::exception();
        }
        if(pthread_detach(m_threads[i])){
            delete[] m_threads;//防止内存泄漏
            throw std::exception();
        }
    }
}                
template<typename T>
threadpool<T>::~threadpool(){
    delete[] m_threads;
    m_stop = true;
}

template<typename T>
bool threadpool<T>::append(T* request){
    //操作工作队列时一定要加锁，因为它被所有线程所共享
    std::lock_guard<std::mutex> lock(m_queuelocker);
    if(m_workqueue.size() > m_max_request){
        return false;
    }
    m_workqueue.push_back(request);
    m_queuestat.post();
    return true;
}

template<typename T>
void* threadpool<T>::worker(void* arg){
    threadpool* pool = (threadpool*) arg;
    pool->run();
    return pool;
}

template<typename T>
void threadpool<T>::run(){
    while(!m_stop){
        m_queuestat.wait();
        std::lock_guard<std::mutex> lock(m_queuelocker);
        if(m_workqueue.empty()){//防止虚假唤醒
            continue;
        }
        T* request = m_workqueue.front();
        m_workqueue.pop_front();
        if(!request){
            continue;
        }
        request->process();
    }
}
#endif