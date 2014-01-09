#include <stdio.h>
#include <stdlib.h>

int main(){
	int pid;
	int x =1;

	pid = fork();
	if(pid == 0){
		printf("\nchild x addr:%p\n",&x);
		
	}

	printf("parent x addr:%p\n",&x);
	
}
