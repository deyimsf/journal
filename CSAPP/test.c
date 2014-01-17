#include <stdio.h>

int fun(int x, int *p){
	return 2;
}

main(){
	int (*fp)(int x,int *p);
	fp = fun;	
	printf("%x\n",fp);
	printf("%x\n\n",fp+1);


	struct S1{
		int i;
		int j;
		double a;
		char c;
	};
	printf("struct S1 size:%d\n\n",sizeof(struct S1));
	
        struct S1 aa;
	int bb;
	printf("----%d , %d\n",sizeof( aa), sizeof( bb));	
   
	int local;
	printf("local at %p\n",&local);
	printf("local at %p\n",&local+1);
	return 0;
	
}


