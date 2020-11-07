#ifndef _LOCKER_H_
#define _LOCKER_H_
#include <exception>
#include <pthread.h>
#include <semaphore.h>

class sem{
public:
    sem(){
        if(sem_init(&m_sem, 0, 0) != 0){
            //构造函数木有返回值，可以通过抛出异常来报告错误
            throw std::exception();
        }
    }
    ~sem(){
        sem_destroy(&m_sem);
    }
    bool wait(){
        return sem_wait(&m_sem) == 0;
    }
    bool post(){
        return sem_post(&m_sem) == 0;
    }

private:
    sem_t m_sem;    

};
#endif