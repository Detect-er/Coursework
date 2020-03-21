#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<wait.h>
#include<stdlib.h>
#include<sys/types.h>

//信号的发送和捕捉,kill()、raise()、alarm()、pause()函数的使用
int main()
{
    pid_t pid;
    int ret;
    if ((pid=fork())<0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid==0)
    {
        printf("Child %d waiting for signal\n",getpid());
        raise(SIGSTOP);//暂停，等待信号
        printf("Child cannot come here!");
        exit(EXIT_SUCCESS);
    }
    else
    {
        sleep(1);
        if ((waitpid(pid,NULL,WNOHANG))==0)     
        {
            if ((ret=kill(pid,SIGKILL))==0)
            {
                printf("Parent %d kills its Child %d",getpid(),pid);
            }
            waitpid(pid,NULL,0);//等待子进程终止
            ret=alarm(3);//3秒后发送SIGALARM信号
            pause();//进程挂起等待信号
            printf("Parent cannot come here");
        }
        
    }
}

