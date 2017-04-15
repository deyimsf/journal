#include <sys/epoll.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>

/*
 epoll学习

*/
int main(){

	//创建一个监听socket
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in sock_addr_in;
	sock_addr_in.sin_family = AF_INET;
	sock_addr_in.sin_port = htons(80);
	inet_aton("127.0.0.1", &sock_addr_in.sin_addr);

	//绑定
	int ret = bind(socket_fd, (struct sockaddr *)&sock_addr_in,sizeof(sock_addr_in));
	if(ret == -1){
		printf("绑定socket错误\n");
	}

	//开始监听
	ret = listen(socket_fd, 1024);
	if(ret == -1){
		printf("监听失败\n");
	}


	int epoll_fd = epoll_create(10);
	struct epoll_event event;
	event.events = EPOLLIN;
	event.data.fd = socket_fd;


	int result = epoll_ctl(epoll_fd, EPOLL_CTL_ADD,
			socket_fd, &event);
	printf("%d\n", result);	


	//就绪事件集合
	struct epoll_event event_list[1024];
	int nevents = 1024;
	int timeout = 100000000;
	int i = 0;
	

	//开始轮训
	for(;;){
		//获取事件
		int events = epoll_wait(epoll_fd, event_list, nevents, timeout);	
		
		for(i = 0; i < events; i++){
			int sock_fd = event_list[i].data.fd;
			
			// 如果这个事件里的fd和监听fd(socket_fd)相同,
			// 代表这个事件是监听套接字(socket_fd)的可读事件
			if(sock_fd == socket_fd && (event_list[i].events & EPOLLIN)){
				//接收一个链接
				struct sockaddr_in client_addr;
				int addrlen;
				int client_fd = accept(socket_fd, (struct sockaddr *)&client_addr, &addrlen);				
				//TODO 这里可以考虑是否把client_fd放入到epoll中				

				write(client_fd, "nihao\0", sizeof("nihao"));
				close(client_fd);
			
			}else{//否则就是其它fd的事件
				printf("======\n");
			}
		}
		
	}
}
