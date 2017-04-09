#include<stdio.h>
#include<string.h>
#include<stdlib.h>


int main(){
	char *a = "aaaa";
	char ****b = "bbbb";
	
	a = b;

	printf("%s %s %d\n", a, b, sizeof(*a));
}
