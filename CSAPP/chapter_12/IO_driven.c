#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h> //IO多路复用

int main(int argc, char **argv){

       int fd_setsize = 2;
       fd_set read_set,ready_set; //IO多路复用的描述符集合   
       int clientfd[fd_setsize]; 
       int maxfd; //最大描述符      
  
       //////初始化
       int listenfd = open_listenfd();
       FD_ZERO(&read_set); //创建一个空的fd_set集合
       FD_SET(STDIN_FILENO,&read_set);  //标准输入放入监控集合
       FD_SET(listenfd,&read_set);  //套接字描述符放入监控集合
       maxfd = listenfd;
       int n;
       for(n=0; n<fd_setsize; n++){
	  clientfd[n] = -1;
       }
       
       ///////轮询select
       while(1){ //这种多路复用的方式，可以让一个进程处理多个IO，和java中的nio一样
	
         ready_set = read_set;  //select返回后这个集合里面就是可以读的描述符
	 select(maxfd+1,&ready_set,NULL,NULL,NULL); //select每次都会修改第二个参数的执指向
         
         //////////////////////接收客户端 add_client
         if(FD_ISSET(listenfd,&ready_set)){
	   struct sockaddr_in clientaddr;  //客户端套接字地址
	   int addrlen;                    //套接字地址的字节个数
	   int connfd = accept(listenfd,(struct sockaddr *)&clientaddr,&addrlen);
	   //将已经建立连接的套接字描述符放入集合
	   int i;
	   for(i = 0; i<fd_setsize; i++){
		if(clientfd[i] < 0){  //如果池中有空位
		  clientfd[i]=connfd;
                  FD_SET(connfd,&read_set);

                  //更新最大描述符
                  if(connfd > maxfd){
			maxfd = connfd;
		  }
	
		  break;
		}
                
      	   } 
           //如果池中没有空位
           if(i == fd_setsize){
	        printf("接收的客户端太多了,丢弃该连接:%d!\n",connfd);
		close(connfd);
           }

	 }
    
         /////////////////处理客户端的可读信息
         int j;
         for(j=0; j<fd_setsize; j++){
	    int connfd = clientfd[j];
	    if((connfd>0) && FD_ISSET(connfd,&ready_set)){ //集合中该描述符是否准备好  
	      	char c[1024];
		int count = read(connfd,c,1023);
                if(count == 0){ //EOF 文件结尾
			close(connfd); //关闭描述符
			FD_CLR(connfd,&read_set); //清除读集合
                        clientfd[j] = -1;  //删除在池中站得位置
			printf("clear connfd: %d\n",connfd);
		}else if(count == -1){
		         printf("read connfd: %d error!",connfd);
		}else{
			c[count] = '\0';
               	 	printf("server received connfd[%d] data: %s\n",connfd,c);
		}
	    }
	 }
       }
}




int open_listenfd(){
	//创建套接字描述符
	int listenfd = socket(AF_INET,SOCK_STREAM,0);
        //指定特定ip地址
	struct in_addr inaddr;        
        inaddr.s_addr = htonl(INADDR_ANY); //主机字节顺序转网络字节顺序，这台主机所有ip地址       
        //服务器socket地址
        struct sockaddr_in serveraddr;
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

	return listenfd;
}
