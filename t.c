#include <stdio.h>
#include <stdlib.h>

int main(){
	void *** *ctx;
	ctx = malloc(3 * sizeof(void ***));
	*ctx = malloc(3 * sizeof(void **));	
	**ctx = malloc(3 * sizeof(void *));	
	//printf("%s\n",*(char**)(***ctx));

	char c, *l, **g;
	c = 'c';
	l = "lll";
	g = malloc(sizeof(char*));
	*g = "ggg";	
	printf("%c %s %s\n",c,l,*g);


	int *a;
	int ***b;
	
	a = malloc(sizeof(int *));

	b = malloc(sizeof(int *));
	*b = malloc(sizeof(int *));
	printf("%d,%d\n",a,b);

	*(int *****)a = b;
	printf("%d,%d\n",a,b);
}
