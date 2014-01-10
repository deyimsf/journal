#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>

void handler(int signal){
	int pid = getpid();
	printf("handler signal:%d ,handler函数的id:%d\n",signal,pid);
}

int main(){
  signal(SIGCHLD,handler); //17
  signal(SIGALRM,handler); //14
    
  //当前进程号
  int pid = getpid();
  printf("parent:%d\n",pid);
   alarm(1); 
  //子进程号
  int chidid = fork();
  //子进程
  if(chidid == 0){
     printf("chidid:%d\n",getpid());
     sleep(1);
     exit(0);
  } 
 

  
  while(1);
}
