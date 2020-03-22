#include<stdio.h>
#include<stdlib.h>
#include<signal.h>

void myfunc(int signum) 
{ 
    printf("myfunc: SIGINT captured.\n"); 
    raise(SIGQUIT);//默认处理方式：终止进程
}

int main() {
    sigset_t newmask;
    sigset_t oldmask;
    sigset_t testmask;
    struct sigaction act;

    act.sa_handler = myfunc;  //信号处理函数为myfunc
    sigemptyset(&act.sa_mask);//mask集合清空
    act.sa_flags = 0;
    sigaction(SIGINT, &act, 0);  //信号捕捉函数，捕捉Ctrl+C

    printf("block before,sleep 5s\n");
    sleep(5);
    //屏蔽SIGINT信号
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGINT);//将SIGINT添加到newmask中
    sigprocmask(SIG_BLOCK, &newmask, &oldmask);//将newmask添加到信号屏蔽字中，保存之前的集合到oldmask
    
    printf("block after,sleep 5s\n");
    sleep (5);//在休眠期，SIGINT信号会被阻塞 

    sigpending(&testmask);//把阻塞的信号放入testmask中
    if (sigismember(&testmask, SIGINT)) //判断testmask集合中是否又sigint信号
        printf("\nSIGINT has been ignored\n");

    printf("Now SIGINT is unblocked\n");
    sigprocmask(SIG_SETMASK, &oldmask, NULL);//将信号屏蔽字设置为oldmask集合
    //此处开始处理信号，调用信号处理函数
    printf("Process cannot come here if running correctly.");
}
