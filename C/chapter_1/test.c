#include <stdio.h>

main(){
	printf("hello\n");
	const char str[] = "hello";
	str[]="a";
	printf("%s\n",str);
	
	//char strs[] = "hello";
    //int i;
	//for(i=0; i<7; ++i){
	//	printf("%d\n",strs[i]);
	//}

	//char *s = "hello";
	//char ss[] = "hello";
	//printf("%s, %s\n",s,ss);	
}
