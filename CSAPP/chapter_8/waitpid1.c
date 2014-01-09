#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <errno.h>
#define N 2

int main(){
	int status,i;
	int pid;
	
	for(i=0; i<N; i++){
		if((pid = fork()) == 0)
			exit(100+i);
	}
        
                    //等待任意一个子进程结束,可以把-1改为特定的pid
	while((pid = waitpid(-1,&status,0))>0){ //status 返回子进程的状态信息
		if(WIFEXITED(status))                                                //解析这个状态信息的退出状态
			printf("child %d terminated normally with exit status=%d\n",pid,WEXITSTATUS(status));
		else
			printf("child %d terminated abnormally\n",pid);
	}
	
	if(errno != ECHILD)
		printf("error\n");

	exit(0);
}	
