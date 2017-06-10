#include <stdio.h>

// 达夫设备减少了变量的比较次数
int main(int argc, char **argv){

	// a是我们任意输入的数字
	// a/5可以决定循环多少次
	// a%5解决余数问题
	int x = 0;
	int a = 5;
	int n = a/5;
	switch(a%5){
		case 0: do{ x++;
		case 4: x++;
		case 3: x++;
		case 2: x++;
		case 1: x++;
		} while (--n>0);
	}
	printf("%d\n",x);



	// 上面的达夫设备,类似下面的代码
	int x1 = 0;
	int a1 = 5;
	int n1 = a1/5;
	do{ 
		x1++;
		x1++;
		x1++;
		x1++;
		x1++;
	}while(--n1 > 0);

	int mod = a1%5;
	while(mod){
		x1++;
		mod--;
	}

	printf("%d\n",x1);
}
