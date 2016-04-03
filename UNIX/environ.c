#include <stdio.h>

extern char **environ;

int 
main(int argc, char **argv){

	int i = 0;
	char *s;
	while(environ[i] != '\0'){
		s = environ[i];
		++i;
			
		printf("%s\n",s);	
	}	

}
