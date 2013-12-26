#include <stdio.h>
#include <stdlib.h>

int fun(int x, int *p){
	return 2;
}

main(){
	/*int (*fp)(int x,int *p);
	fp = fun;	

	printf("%x\n",fp);
	printf("%x\n",fp+1);

	struct S1{
		int i;
		int j;
		double a;
		char c;
	};

	printf("%d\n",sizeof(struct S1));
	*/

	int local;
	printf("local at %p\n",&local);
	printf("local at %p\n",&local+1);
	return 0;
	
}


