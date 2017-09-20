#include <stdio.h>
#include <sys/time.h>

int main(){
	struct timeval time[1];
	int status = gettimeofday(time,NULL);
	
	if(status < 0){
                 return -1;
         }
         
         long millis = (long)time[0].tv_sec * 1000 + (long)time[0].tv_usec / 1000;
    
         printf("%lu\n",millis);
	

	struct timeval a_time[1];
	struct timeval *aa_time[1];
	
	aa_time[0] = a_time;
}
