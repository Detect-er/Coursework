#include "stdio.h"
#include <stdlib.h>
#include <pthread.h>
#include<semaphore.h>

#define N_WRITER 3 //写者数目
#define N_READER 6 //读者数目
#define W_SLEEP 1 //控制写频率
#define R_SLEEP  0.5 //控制读频率

pthread_t wid[N_WRITER],rid[N_READER];
int data = 0;
int readerCnt = 0, writerCnt = 0;
sem_t sem_read;
sem_t sem_write;
pthread_mutex_t mutex_write;
pthread_mutex_t mutex_read;

void write()
{
    int rd = rand();
    printf("write %d\n",rd);
    data = rd;
}
void read()
{
    printf("read %d\n",data);
}
void * writer(void * in)
{
    sem_wait(&sem_write);
    {//临界区,希望修改 writerCnt,独占 writerCnt
        writerCnt++;
        if(writerCnt == 1){
            //阻止后续的读者加入待读队列
            pthread_mutex_lock(&mutex_read);
        }
    }
    sem_post(&sem_write);

    pthread_mutex_lock(&mutex_write);
    {//临界区，限制只有一个写者修改数据
        printf("写线程id%d进入数据集\n",pthread_self());
        write();
        printf("写线程id%d退出数据集\n",pthread_self());        
    }
    pthread_mutex_unlock(&mutex_write);

    sem_wait(&sem_write);
    {//临界区,希望修改 writerCnt,独占 writerCnt
        writerCnt--;
        if(writerCnt == 0){
            //阻止后续的读者加入待读队列
            pthread_mutex_unlock(&mutex_read);
        }
    }
    sem_post(&sem_write);
    sleep(W_SLEEP);

    pthread_exit((void *) 0);
}

void * reader (void * in)
{
    //假如写者锁定了mutex_read,那么成千上万的读者被锁在这里
    pthread_mutex_lock(&mutex_read);//只被一个读者占有
    {//临界区
        sem_wait(&sem_read);//代码段 1
        {//临界区
            readerCnt++;
            if(readerCnt == 1){
                pthread_mutex_lock(&mutex_write);
            }
        }
        sem_post(&sem_read);
    }
    pthread_mutex_unlock(&mutex_read);//释放时,写者将优先获得mutex_read

    printf("读线程id%d进入数据集\n",pthread_self());
    read();
    printf("读线程id%d退出数据集\n",pthread_self());

    sem_wait(&sem_read);//代码段2
    {//临界区
        readerCnt--;
        if(readerCnt == 0){
            pthread_mutex_unlock(&mutex_write);//在最后一个并发读者读完这里开始禁止写者执行写操作
        }
    }
    sem_post(&sem_read);

    sleep(R_SLEEP);

    pthread_exit((void *) 0);
}

int main()
{
    printf("多线程,写者优先\n");
    pthread_mutex_init(&mutex_write,NULL);
    pthread_mutex_init(&mutex_read,NULL);
    sem_init(&sem_write,0,1);
    sem_init(&sem_read,0,1);
int i = 0;
    for(i = 0; i < N_READER; i++)
    {
        pthread_create(&rid[i],NULL,reader,NULL);
    }
    for(i = 0; i < N_WRITER; i++)
    {
        pthread_create(&wid[i],NULL,writer,NULL);
    }
    sleep(1);    
    return 0;
}
