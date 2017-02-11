#include <stdlib.h>
#include <stdio.h>


int main(int argc, char *argv[]){
	char *name = malloc(30);
	*name = 'a';		

	printf("name = %s \n",name);

	free(name);
}
