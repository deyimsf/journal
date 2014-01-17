#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

int main(int argc, char **argv){
	//创建套接字描述符
	int listenfd = socket(AF_INET,SOCK_STREAM,0);

	//int optval=1;
	//setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,(const void *)&optval,sizeof(int))//设置socket一些配置
	
        //指定特定ip地址
	struct in_addr inaddr;        
	if(inet_aton("10.12.212.86", &inaddr) == 0){ //此时已经是网络字节顺序
		printf("点分十进制ip转网络ip地址错误!");
		return -1;
	}
        //所有ip地址
        //inaddr.s_addr = htonl(INADDR_ANY); //主机字节顺序转网络字节顺序，这台主机所有ip地址       
	
        //服务器socket地址
        struct sockaddr_in serveraddr;
        //从指定地址开始为套接字清零，第二个参数是要清零的字节个数
	//bzero((char *)&serveraddr,sizeof(serveraddr));
        serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(8001);    //主机字节顺序到网络字节顺序转换
	serveraddr.sin_addr.s_addr = inaddr.s_addr;
 
        //listenfd描述符和服务端套接字地址绑定
        int bi = bind(listenfd,(struct sockaddr *)&serveraddr,sizeof(serveraddr));
        if(bi < 0){
		printf("绑定失败！\n");
		return -1;
	}
        
        //开始监听
        int li = listen(listenfd,1024);        
        if(li < 0){
		printf("监听失败");
                return -1;
	}

       //等待接受客户端请求
       struct sockaddr_in clientaddr;  //客户端套接字地址
       int addrlen;                    //套接字地址的字节个数
       int connfd = accept(listenfd,(struct sockaddr *)&clientaddr,&addrlen); 
 
       //从套接字描述符读数据
       while(1){
	  char c[]={0,'\0'};
          read(connfd,c,1);
          printf("%s",c);	
	}
       //关闭 套接字描述符
       close(connfd);
       close(listenfd);	
}

