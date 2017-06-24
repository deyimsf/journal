#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

/*
 * 测试子进程是否会继承在主进程中设置的信号函数
 * 
 * 结论是会继承
 */     


 void func(int sig){
	//打印当前进程id
      	printf("process id: %d\n", getpid());
 }    

 int main(int argc, char** argv){

	struct sigaction sa;
	sa.sa_handler = func;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);


 	int pid = fork();

	if (pid == -1 ) {
		printf("fork子进程时错误\n");
		return -1;
	}


	// 子进程
	if (pid == 0) {
		while(1){
			pause();
			printf("子进程[%d] 接收到一个信号\n",getpid());
		}
	}


	// 主进程
	while(1){
		pause();
		printf("父进程[%d] 接收到一个信号\n", getpid());
	}

 }

