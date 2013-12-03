#include <stdio.h>
#include <stdlib.h>  //atof()函数

#define MAXOP 100    //操作数或运算符的最大长度
#define NUMBER '0'   //表示找到一个数字

/*声明函数原型*/
int getop(char s[]); //获取一个操作数，返回'0'表示成功获取,s[]里放的就是
void push(double d);
double pop(void);

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

//栈操作
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


//获取运算符或操作数
#include <ctype.h>
int getch(void);
void ungetch(int);

/*获取操作数*/
int getop(char s[]){
	int i, c;
	while((s[0] = c = getch()) == ' ' || c == '\t'){  //过滤掉空格和制表符
	}
	s[1] = '\0';
	
	if(!isdigit(c) && c != '.'){ //非数字
		return c;
	}		
	i=0; //此时，s[0] 已经是一个数字
        if(isdigit(c)){ //是数字，收集整数部分
                while(isdigit(s[++i] = c = getch())){//取下一个数字
                }
        }       
        if(c == '.'){//收集小数部分
                while(isdigit(s[++i] = c = getch())){
                }
        }
        
        s[i] = '\0'; //重置最后一个多出的非数字字符
        if(c != EOF){ //如果最后一个字符不是EOF，就将其还回去
                ungetch(c);
        }

        return NUMBER;
}

//取一个字符或还回一个字符
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
