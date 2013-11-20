#include <stdio.h>

main(){
	//const char cc[] = "hello"; 数组值不可修改
        char cc[] = "hello";

	cc[0] = 'l';
	printf("%s\n",cc);

	return 0;
}
