#include <stdio.h>

main(){
	printf("%d,%d,%d",EOF,'\n',"\n");


	int c;
	c = getchar();
	while(c != EOF){
		putchar(c);
		c = getchar();
	}
	
}
