#include<stdio.h>
#include<stdlib.h>
#include<signal.h>

void myfunc(int signum)
{
    if(signum==SIGALRM) 
		printf("SIGALRM captured,signum=%d\n",signum);
    else if(signum==SIGQUIT)  
    	printf("SIGQUIT captured.\n");
	else if(signum==SIGINT)
    	printf("SIGINT captured.\n"); 
}

int main()
{
    int i=0;
    signal(SIGALRM,myfunc);
    alarm(3);
    while (i<5)
    {
        i++;
        sleep(1);
        printf("process running %ds\n",i);
    }

    signal(SIGQUIT,myfunc); // ctrl+\ 
    signal(SIGINT,myfunc);  //ctrl+c
    pause();//挂起进程，等待信号
    exit(EXIT_SUCCESS);
}