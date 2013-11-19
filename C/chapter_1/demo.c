#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_COLS 20
#define MAX_INPUT 1000

/*
 枚举 enum Jar_Type{CUP,PINT,QUART,HALF_GALLON,GALLON}; 
 */

/*
 int main(){
	//字符串
	char input[MAX_INPUT];
	
	while( gets(input) != NULL){
		printf("字符串: %s\n",input);
	}
}*/

int main(){
	//指针
	//把字符串的第一个字符的地址给*m
	//像char m = "abcd"是不允许的，因为char是字符
	//没有办法显示的为指针赋值，如char *m = 0x7fff23456743
        char *m = "abcd"; 
        
	printf("指针值%%x(16):%x\n",&m);
	printf("指针值%%p(16):%p\n",&m);
	printf("指针值%%o(8):%o\n",&m);
	printf("指针值%%d(10):%d\n",&m);
	
}
