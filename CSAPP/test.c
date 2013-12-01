#include <stdio.h>

void cc(int*,int*);
main(){
	int x=2;
	int *y = &x;

	printf("x: %x\n",x);
	printf("*y: %x\n",*y);printf("*y: %x\n",*y);

	printf("y: %x\n",y);
	printf("++y: %x\n",++y);
	printf("*y: %x\n",*y);

}


