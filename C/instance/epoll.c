#include <sys/epoll.h>
#include <stdio.h>
#include <stdlib.h>


/*
 epoll学习

*/
int main(){

	int epoll_fd = epoll_create(10);
	printf("%d\n", epoll_fd);

	int sockfd = 4;
	struct epoll_event event;
	event.events = EPOLLIN;
	event.data.fd = sockfd;

	int result = epoll_ctl(epoll_fd, EPOLL_CTL_ADD,
			sockfd, &event);
	printf("%d\n", result)	

}
