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


	typedef struct {
		int	 age;
	} person_t;

	// 子进程
	if(pid == 0){
		person_t sub;

		struct iovec iov[1];
		struct msghdr	msg;
		
		while(1){
			
			// 用于接收数据的缓冲区开始地址
			iov[0].iov_base = (char *)&sub;
			// 缓冲区长度
			iov[0].iov_len = sizeof(person_t);
			
			msg.msg_name = NULL;
			msg.msg_namelen = 0;
			msg.msg_control = NULL;
			//msg.msg_controllen = 0;
			
			// 设置接收数据的缓冲区数组
			msg.msg_iov = iov;
			msg.msg_iovlen = 1;
			

			int n = recvmsg(sockets[1], &msg, 0);
			if(n == -1){ //出错了
				sleep(1);
				write(STDOUT_FILENO, "error", sizeof("error"));
			}	
			
			// 输出结果
			printf("接收数据: %d个字节, 值是%d\n",n, sub.age);
		}
	}


	// 主进程
	while(1){
		person_t main;
		main.age = 23;
		
		struct iovec iov[1];
		struct msghdr msg;

		iov[0].iov_base = (char *)&main;
		iov[0].iov_len = sizeof(main);

		msg.msg_name = NULL;
		msg.msg_namelen = 0;
		msg.msg_control = NULL;
		
		msg.msg_iov = iov;
		msg.msg_iovlen = 1;
		
		printf("\n>>发送数据%zu\n", iov[0].iov_len);	
		sendmsg(sockets[0], &msg, 0);
		// 每隔一秒发送一个person_t结构体
		sleep(1);
	}

}
