#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

     

 void func(int sig){
	for(int i = 0; i < 5; i++){
      		printf("--\n");
		// 休息两秒
		sleep(2);
	
		// 检测信号函数在执行时是否会屏蔽掉后续信号
	}
 }    

 int main(int argc, char** argv){
         sigset_t set; 
         sigset_t set_empty;
     
         // 清空这两个信号集合 
         sigemptyset(&set);
         sigemptyset(&set_empty);

	 //添加一个键盘终端信号(ctrl + c)
         sigaddset(&set, SIGINT);

	 //为当前进程的绑定SIGINT信号,并为这个信号绑定一个方法
         struct sigaction sa;
         sa.sa_handler = func;
         sigemptyset(&sa.sa_mask);
         sigaction(SIGINT, &sa, NULL);

         //首先为进程屏蔽绑定SIGINT信号
         sigprocmask(SIG_BLOCK, &set, NULL);


         while(1){
               // 不停的向标准输出中输出*号
               write(1, "*", strlen("*"));

               // 临时放过set_empty集合之外的信号,等执行完一个信号后
               // 再恢复到原来的信号屏蔽值,这样每接收一个SIGINT信号
               // 就会打印一个 "--\n",如果没有任何信号,则会一直阻塞
        #if 1
                sigsuspend(&set_empty);
	#else
                // 撤销对该进程的set中的信号屏蔽
                sigprocmask(SIG_UNBLOCK, &set, NULL);

                // 每隔一秒钟打印一个*号
                // 此时,每发送一次SIGINT信号就会发送一个"--n"并提前
                // 打断sleep(会提前醒来)
                        sleep(1);
        #endif
                }

        }

