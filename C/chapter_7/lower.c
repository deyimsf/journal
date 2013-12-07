#include <stdio.h>
#include <ctype.h>

main(int argc ,char *argv){
	int c;
	while((c=getchar()) != EOF){
		putchar((tolower(c)));
	}
	return 0;
}


