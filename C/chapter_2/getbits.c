#include <stdio.h>

unsigned int getbits(unsigned,int,int);

main(){
	printf("%d\n",getbits(127,4,3));
}

/*
 * x 
 * p x这个数从右数的位置  是二级制位的位置
 * n 要截取的二进制位的长度
 * */
unsigned int getbits(unsigned x,int p, int n){
	return (x >> (p-n+1) & ~(~0 << n)); // 0按位求反，则所有位都是1,类似 1111 1111,左移2位变成这样 1111 1111 00
}
