#include <stdio.h>

main(){
	printf("%c\n",'\\');
	printf("%c %d\n",'\x7','\x7');
	
	// '\000' 用八进制表示一个字符 如'\013'
	// '\xhh' 用十六进制表示一个字符 如'\xb'
}
