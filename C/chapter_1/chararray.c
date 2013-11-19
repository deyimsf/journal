#include <stdio.h>
#define MAXLINE 1000 //允许输入的最大长度

int getlin(char s[],int lim);
void copy(char to[], char from[]);

main(){
	int len;
	int max;
	char line[MAXLINE];
	char longest[MAXLINE];
	
	max = 0;
	while((len = getlin(line,MAXLINE)) > 0){
		if(len > max){
			max = len;
			copy(longest,line);
		}
	}

	if(max > 0){
		printf("%s",longest);
	}

	return 0;
}


//将一行读入到s中，并返回其长度
int getlin(char s[], int lim){
	int c,i;
	for(i=0; i<lim-1 && (c=getchar()) != EOF && c !='\n'; ++i){
		s[i] = c;
	}
	if(c == '\n'){
		s[i] = c;
		++i;
	}
	s[i] = '\0';
	return i;
}

//函数拷贝
void copy(char to[], char from[]){
	int i;
	while((to[i] = from[i]) != '\0'){
		++i;
	}

}



