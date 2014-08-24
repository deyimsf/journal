//#include <stdio.h>
//#include <stdlib.h>
//#include <fcntl.h>
//#define PERMS 0666 //对于所有者、所有者组和其他成员均可读写

int aa();

main(){
	char buf[200];
	printf("hello\n");
	read(7,buf,99);	
	aa();
	printf("world\n");
}

