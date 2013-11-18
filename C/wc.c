#include <stdio.h>
/*定义常量*/
#define IN 1 /*在单词内*/
#define OUT 0 /*在单词外*/

main(){
	int c,nl,nw,nc,state;
	state = OUT;
	nl = nw = nc = 0;
	while((c = getchar()) != EOF){
		//字符数
		++nc;
		//行数
		if(c == '\n'){
			++nl;	
		}
		//单词数
		if(c == ' ' || c == '\n' || c == '\t'){
			state = OUT;

		}else if(state == OUT){
			state = IN;
			++nw;
		}
	}
	printf("%d %d %d\n",nl,nw,nc);
}
