#include <stdio.h>

typedef unsigned char *byte_pointer; //typedef为数据类型命名；命名一个指向char类型的指针

//打印字节,将某个数的起始地址，和这个数的字节长度传入
void show_bytes(byte_pointer start ,int len){
	int i;
	for(i = 0; i < len; i++){
		printf("%.2x ",start[i]);
	}
	printf("\n");
}

void show_int(int x){
	show_bytes((byte_pointer) &x, sizeof(int));
}

void show_float(float x){
	show_bytes((byte_pointer)&x, sizeof(float));
}

void show_pointer(void *x){
	show_bytes((byte_pointer) &x, sizeof(void *));
}


main(){
	int val = 12345;  //0x00003039
	
	int ival = val;
	float fval = (float)ival;
	int *pval = &ival;

	show_int(ival);
	show_float(fval);
	show_pointer(pval);

        unsigned char *s = "abcdef";
	show_bytes(s, 6);

}
