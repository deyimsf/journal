 #include <stdio.h>
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <stdlib.h>
  #include <netinet/in.h>
  #include <string.h>
  #include <poll.h>
  #include <unistd.h>
  #include <fcntl.h>

 
int main(int argc,char *argv[]){
	//校验输入参数
	if(argc < 3){
		printf("errored arguments\n");
	}
	
	char *ip = argv[1];
	int port = atoi(argv[2]);
	int socketfd = socket(AF_INET,SOCK_STREAM,0);
	//- int flag = fcntl(socketfd,F_SETFL,O_NONBLOCK); //将文件设置为非阻塞方式	

	struct in_addr inaddr;
	int ret = inet_aton(ip,&inaddr); //ip地址
	if(ret == -1){
		perror("inet_aton");	
		return 2;
	}

	struct sockaddr_in serveraddr;	
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port = htons(port);
	serveraddr.sin_addr = inaddr;
	
	//int connect(int sockfd,struct sockaddr *serv_ddr,int addrlen)
	ret = connect(socketfd,(struct sockaddr *)&serveraddr,sizeof(serveraddr));
	//- 如果将socketfd设置为了非阻塞方式,并且链接没准备好
	//- 那么connect的返回值会是-1
	if(ret == -1){
		//- 如果socketfd为非阻塞方式并且打印Operation now in progress
		//- 那么说明连接正在进行，还没准备好
		//- 具体返回-1的原因可根据其他方式判断
		perror("connect"); 
		return 1;
	}

	int buffer=1;
	char buf[buffer];
	int len;

	//IO多路复用	
	struct pollfd fds[2];
	fds[0].fd = STDIN_FILENO;
	fds[0].events = POLLIN;
	fds[1].fd = socketfd;
	fds[1].events=POLLIN;

	while(1){
		int ret = poll(fds,2,5000); //超时时间5秒
		if(ret == -1){
			perror("poll");
          return 1;
        }    
             
        //读标准输入数据 
        if(fds[0].revents & POLLIN){
            len = read(0,buf,buffer);
            if(len < 0){
                perror("read(stdin)");
            }    

            write(socketfd,buf,len);
        }    

        //读socket数据
        if(fds[1].revents & POLLIN){
            //- 走到这里说明fd数据已经准备好,可以读 
            //- 1.如果是fd是阻塞方式,那么将准备好的数据读完后再读一次则该方法会阻塞 
            //- 2.如果fd是非阻塞方式,准备好的数据读完后再读一次则该方法不会阻塞 
            len = read(socketfd,buf,buffer);
            if(len < 0){
                perror("read(socketfd)");
                break;
            }    
            if(len == 0){
                break;
            }    
            //向标准输出写数据   
            write(1,buf,len);
        }    
    }    
 }           

