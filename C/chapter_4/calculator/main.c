#include <stdio.h>
#include <stdlib.h> //atof()函数
#include "calc.h"
#define MAXOP 100

/*逆波兰计算器*/
main(){
	int type; //+ - * / 0
	double op2; //操作数
	char s[MAXOP]; //操作数

	while((type = getop(s)) != EOF){
		switch(type){
		case NUMBER: //是数字
			push(atof(s)); //推入栈顶
			break;
		case '+':
			push(pop() + pop());
			break;
		case '*':
			push(pop() * pop());
			break;
		case '-':
			op2 = pop();
			push(pop() - op2);
			break;
		case '/':
			op2 = pop();
			if(op2 == 0.0){
				printf("error: zero divisor\n");
				break;
			}
			
			push(pop() / op2);
			break;
		case '\n':
			printf("\t%.8g\n",pop());
			break;
		default:
			printf("error: unkown command %s\n", s);
			break;
		}
	}
}
