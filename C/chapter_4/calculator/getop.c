#include <ctype.h>
#include <stdio.h>
#include "calc.h"

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


