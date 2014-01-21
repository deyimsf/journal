#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h> //IO多路复用

int main(int argc, char **argv){
       int listenfd;
       fd_set read_set,ready_set; //I/O多路复用的描述符集合    
 
       listenfd = open_listenfd();
       FD_ZERO(&read_set); //创建一个空的fd_set集合
       FD_SET(STDIN_FILENO,&read_set);  //标准输入放入监控集合
       FD_SET(listenfd,&read_set);  //套接字描述符放入监控集合
      
       //轮询select
       while(1){ //这种多路复用的方式，可以让一个进程处理多个IO
	
         ready_set = read_set;  //select返回后这个集合里面就是可以读的描述符
	 select(listenfd+1,&ready_set,NULL,NULL,NULL); //select每次都会修改第二个参数的执指向
        
         printf("=================================================\n"); 
         //如果准备好集合中有这个描述符
         if(FD_ISSET(STDIN_FILENO,&ready_set)){
	    printf("标准输入描述符:%d 已准备好，可以读了!\n",STDIN_FILENO);
	   
            char c[2];
	    read(STDIN_FILENO,&c,2);
	    printf("-----> %s\n",&c);
           
	 } 
         
         //
         if(FD_ISSET(listenfd,&ready_set)){
	    printf("listenfd :%d 准备好了，可以调用accept方法了!\n",listenfd); 
	    //accept();
	 }
         printf("-----------------------------------------------------\n"); 

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
