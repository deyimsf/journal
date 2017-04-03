#include<stdio.h>
#include<string.h>
#include<stdlib.h>


int main(){

	char *value = getenv("JAVA6_HOME");
	printf("%s\n", value);
}
