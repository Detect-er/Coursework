#include <stdio.h>
#include <stdlib.h>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <sys/types.h>
// #include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int main()
{
    int fd_fifo,fd_file;  //读取管道fd_fifo中的内容，写到文件fd_file中
    char buf[50] = {0};//buf存放读出的内容
    ssize_t size;
    
    fd_fifo = open("./fifo",O_RDONLY);//读方式打开管道
    if(fd_fifo < 0)
    {
        perror("open fifo fail: ");
        exit(1);
    }

    //把从有名管道中读取的数据，写到文件argv[1]中，返回fd_file
    fd_file = open("./client2.txt",O_WRONLY|O_CREAT|O_TRUNC,0666);
    if(fd_file < 0)
    {
        perror("fd_w open fail ");
        exit(1);
    }

    //fifo 中循环读取数据，然后写到文件中
    while(1)
    {
        size = read(fd_fifo,buf,50); //读有名管道内容存入buf，返回值为读取了size个字节的数据
        if(size <= 0)
            break;
        write(fd_file,buf,size); //写入文件argv[1]中
    }
    close(fd_fifo);
    close(fd_file);
    return 0;
}