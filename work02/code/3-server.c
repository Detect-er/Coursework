//创建fifo管道，并将source.txt里的内容存入管道中
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <sys/types.h>
// #include <sys/stat.h>
#define BUFFSIZE 50

int main(int argc, const char *argv[])
{
    //无名管道变量
    int pfd[2]; //[0]为读取端，[1]为写入端
    int pipe(pfd);
    char pipe_buf[BUFFSIZE]={0};
    if(pipe(pfd)<0)
    {
        perror("pipe error"); exit(1);
    }
    pid_t pid;
    pid=fork();

    if (pid==0)
    {
        sleep(2);
        close(pfd[1]); 
        int rdnum=read(pfd[0],pipe_buf,BUFFSIZE); 
        printf("Server's child process.Read %d bytes from the pipe:%s\n",rdnum,pipe_buf); 
        close(pfd[0]);
        exit(0);
    }
    else
    {
        //无名管道通信
        close(pfd[0]);
        printf("Server Process.Please input:");
        fgets(pipe_buf,BUFFSIZE,stdin);
        pipe_buf[strlen(pipe_buf)-1]=0;
        int wrnum=write(pfd[1],pipe_buf,strlen(pipe_buf));
        printf("server write %d bytes\n",wrnum);
        close(pfd[1]);
        waitpid(pid,NULL,0);

        //有名管道的通信
        int fd_fifo,fd_file;
        char buf[50] = {0};
        ssize_t size; 
        //1.创建管道
        if(mkfifo("./fifo",0777) != 0 ) //创建，权限为读写执行
        {
            if(errno == EEXIST)
            {
                printf("Fifo file exists\n");
            }
            else
            {
                perror("mkfifo fail ");
                exit(1);
            }
        }
       
        //2.只写打开管道，open调用将被阻塞，直到有一个进程以只读方式打开同一个FIFO文件为止
        fd_fifo = open("./fifo",O_WRONLY);//只写方式打开
        if(fd_fifo < 0)
        {
            perror("open fifo fail: ");
            exit(1);
        }

        //3.将source.txt里的内容复制到管道文件中
        fd_file = open(argv[1],O_RDONLY);//只读方式打开source.txt文件
        if(fd_file < 0)
        {
            perror("open source fail ");
            exit(1);
        }
        printf("source file exists\n");
        while(1)//循环读取文件内容
        {
            size = read(fd_file,buf,50); //读取50 bytes到buf中，返回实际读取的字节数
            if(size <= 0)
            {
                break;
            }
            write(fd_fifo,buf,size);//将buf中的size个bytes写进管道文件
        }
        
        close(fd_file);//关闭读的源文件
        close(fd_fifo);
        return 0;
    }   
}