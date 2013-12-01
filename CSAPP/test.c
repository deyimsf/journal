#include <stdio.h>

main(){
	int  x=2;
	int* y = &x; //y是一个整型指针(地址),&x是一个整型指针(地址) *y是y这个地址所指向的内容，是个整型.

	printf("%d\n",y);     //表示地址
	printf("%d\n",&x);    //表示地址
	printf("%d\n",&(*y)); //表示*y这个整型的地址

	printf("%d\n",*y);    //内存地址空间的内容
	printf("%d\n",*(&x)); //内存地址空间的内容
}


