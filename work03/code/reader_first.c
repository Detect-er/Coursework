//读者优先

#include "stdio.h"
#include <stdlib.h>
#include <pthread.h>
#include<semaphore.h>

#define N_WRITER 3 //写者数目
#define N_READER 6 //读者数目
#define W_SLEEP  1 //控制写频率
#define R_SLEEP  1 //控制读频率

pthread_t wid[N_WRITER],rid[N_READER];
pthread_mutex_t mutex_write;//同一时间只能一个人写文件,互斥
sem_t sem_read;//同一时间只能有一个人访问 readerCnt
int data = 0;
int readerCnt = 0;
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
    pthread_mutex_lock(&mutex_write);
    printf("写线程id%d进入数据集\n",pthread_self());
    write();
    printf("写线程id%d退出数据集\n",pthread_self());
    pthread_mutex_unlock(&mutex_write);
    sleep(W_SLEEP);
        
    pthread_exit((void *) 0);
}

void * reader (void * in)
{
    sem_wait(&sem_read);
    readerCnt++;
    if(readerCnt == 1){
        pthread_mutex_lock(&mutex_write);
    }
    sem_post(&sem_read);
    printf("读线程id%d进入数据集\n",pthread_self());
    read();
    printf("读线程id%d退出数据集\n",pthread_self());    
    sem_wait(&sem_read);
    readerCnt--;
    if(readerCnt == 0){
        pthread_mutex_unlock(&mutex_write);
    }
    sem_post(&sem_read);
    sleep(R_SLEEP);

    pthread_exit((void *) 0);
}

int main()
{
    printf("多线程,读者优先\n");    
    pthread_mutex_init(&mutex_write,NULL);
    sem_init(&sem_read,0,1);
    int i = 0;
    for(i = 0; i < N_WRITER; i++)
    {
        pthread_create(&wid[i],NULL,writer,NULL);
    }
        for(i = 0; i < N_READER; i++)
    {
        pthread_create(&rid[i],NULL,reader,NULL);
    }
    sleep(1);
    return 0;
}
