#include <stdio.h>

main(){
	printf("hello\n");
	char *str = "hello";
	printf("%c\n\n",str[0]);
	
	char strs[] = "hello";
	int i;
	for(i=0; i<7; ++i){
		printf("%d\n",strs[i]);
	}

	char *s = "hello";
	char ss[] = "hello";
	printf("%s, %s\n",s,ss);	
}
