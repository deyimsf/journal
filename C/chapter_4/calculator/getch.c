#include <stdio.h>
#define BUFSIZE 100

char buf[BUFSIZE]; //缓冲
int bufp = 0; //缓冲区中下一个空闲位置

//取一个操作数
int getch(void){
	return (bufp > 0)? buf[--bufp] : getchar();
}
//还回一个字符
void ungetch(int c){
	if(bufp >= BUFSIZE){
		printf("ungetch: too many characters\n");
	}else{
		buf[bufp++] = c;
	}
}
