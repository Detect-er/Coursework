#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int global_v=0;//global variable
//创建fork()、vfork()，输出不同进程的pid，观察比较全局变量global_v与局部变量local_v的变化
int main()
{
	int status1,status2;
	pid_t child1,child2,cpid1,cpid2;//cpid1:captured pid of child1
	int local_v=0;//local variable
	char *args[]={"echo","This is in the execve process\n****************\n",NULL};

	if ((child1=fork())==-1)
	{
		perror("fork failed!");
		exit(EXIT_FAILURE);
	}

	//enter fork() child process
	else if (child1==0)
	{
		global_v++;
		local_v++;

		printf("The child1 process: pid=%d, ppid=%d\nc1-global variable:%d\nc1-local variable:%d\n",\ 
				getpid(),getppid(),global_v,local_v);
		
		puts("**************************");
		exit(EXIT_SUCCESS);
	}

	else
	{
		if ((child2=vfork())==-1)
		{
			perror("vfork failed!");
			exit(EXIT_FAILURE);
		}
		else if (child2==0)
		{
			global_v++;
			local_v++;
			printf("The child2 process: pid=%d, ppid=%d,\nc2-global variable:%d\nc2-local variable:%d\n",\ 
					getpid(),getppid(),global_v,local_v);
			puts("**************************");
			execve("/bin/echo",args,NULL);
			puts("The program should not come here!");
		}
		else
		{
			global_v++;
			local_v++;

			printf("The parent process:pid=%d, ppid=%d\nglobal variable:%d\nlocal variable:%d\n",\ 
					getpid(),getppid(),global_v,local_v);
			puts("**************************");
			
			cpid1=wait(&status1);
			cpid2=wait(&status2);
			printf("capture child1's pid:%d,child2's pid:%d",cpid1,cpid2);
		}

	}

	return 0;
}

