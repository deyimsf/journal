#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

main(){	
	int fd = open("aa.txt",O_RDWR|O_CREAT|O_APPEND,0);
	char aa;
	printf("%d\n",read(fd,&aa,1));
	write(fd,&aa,1);
	printf("fd:%d\n",fd);
}
