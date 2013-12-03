#include <stdio.h>
#define MAXVAL	100 //栈的最大深度

int sp = 0; //栈的下一个空闲位置
double val[MAXVAL]; //栈

/*压栈操作*/
void push(double d){
	if(sp > MAXVAL){
		printf("error: stock full, can't push %g\n", d);
		return;
	}

	val[sp++] = d;
}

/*弹出操作*/
double pop(void){
	if(sp > 0){
		return val[--sp];
	}

	printf("error: stack empty\n");
	return 0.0;
}


