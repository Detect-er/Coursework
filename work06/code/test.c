#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>

int main(){
int fd, retval;
char W_buffer[10];
char R_buffer[10];
fd=open("/dev/scull", O_RDWR);

retval = ioctl(fd, 1, 0);
printf("run successfully\n");
retval = write(fd, W_buffer, 10);
printf("write is %d\n", retval);
retval = lseek(fd, 0, 0);
retval = read(fd, R_buffer, 10);
printf("read is %d\n", retval);
close(fd);
return 0;
}
