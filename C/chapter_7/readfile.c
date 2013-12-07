#include <stdio.h>

//读文件
main(int argc ,char *argv[]){
	FILE *fp; //指向文件的指针
	void filecopy(FILE *infp,FILE *outfp);
	
	if(argc !=2){
		printf("命令错误，请输入文件名!\n");
		return 1;
	}

	fp = fopen(argv[1],"r"); //打开一个文件
	if(fp == NULL){ //如果发生错误，fopen返回NULL
		printf("文件：%s打开错误!\n",argv[1]);
		return 1;
	}

	int c;
	while((c=getc(fp)) != EOF){ //从文件fp中读一个字符
		putc(c,stdout); //向文件stdout中写入一个字符
	}
	
	fclose(fp); //关闭文件，每个操作系统对程序打开的文件个数都有限制

	return 0;
}
