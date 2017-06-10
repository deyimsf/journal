#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>


int main(int argc, char **argv){
	
	// UNIX套接字
	int domain = AF_UNIX;
	int type = SOCK_STREAM;
	int protocol = 0;
	int sockets[2];	

	if(socketpair(domain, type, protocol, sockets) == -1){
	
		printf("创建套接字对错误[%d]\n", errno);	
		exit(-1);
	}


	// fork子进程
	int pid = fork();
	
	if(pid == -1){
		printf("fork子进程是错误\n");
		return -1;
	}


	// 子进程
	if(pid == 0){
		char buf[1];

		while(1){
			// 一直读,每次读一个字符
			ssize_t n = read(sockets[1], buf, sizeof(buf));	
			if(n < 1){
				sleep(1);
			}		

			// 读完一个字符就输出到标准输出
			write(STDOUT_FILENO, buf, sizeof(buf));	
		
			write(STDOUT_FILENO, "\n", sizeof("\n"));
		}
	}


	// 主进程
	int i = 0;
	char *str = "hello";
	while(1){
		write(sockets[0], str, strlen(str));		
	
		// 每隔一秒发送一段字符串
		sleep(1);
	}

}
