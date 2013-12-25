#include <stdio.h>
#include <stdlib.h>

int fun(int x, int *p){
	return 2;
}

main(){
	int (*fp)(int x,int *p);
	fp = fun;	

	printf("%x\n",fp);
	printf("%x\n",fp+1);
}


