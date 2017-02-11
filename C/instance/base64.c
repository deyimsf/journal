#include <stdio.h>

//BASE64字符
static char pem_array[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/' };

static char getCharIndex(unsigned char);	

int main(int argv,char **argc){
	char *str = "1234567890";
	int length = 0;
	char *p = str;
	while(*p++ != '\0'){
		length++;
	}

	//编码	
	int index = 0;
	int mod = length % 3;
	int reset = length - mod;

	while(index < reset){
		 char one = str[index];
		 char two = str[index+1];
		 char three = str[index+2];
		// printf("=%d=",one >> 2 & 0xFF);		
		//0x3F 0011 1111
		//0x30 0011 0000
		//0x0F 0000 1111
		//0x3C 0011 1100
		//0x03 0000 0011
		printf("%c",pem_array[one >> 2 & 0x3F]);
		printf("%c",pem_array[(one << 4 & 0x30)+(two >> 4 & 0x0F)]);
		printf("%c",pem_array[(two << 2 & 0x3C)+(three >> 6 & 0x3)]);
		printf("%c",pem_array[three & 0x3F]);
		index+=3;
		if(index % 57 == 0){
			printf("\n");
		}
	}	

	if(mod == 1){
		char one = str[index];
		char two = 0;
		printf("%c",pem_array[one >> 2 & 0x3F]);
		printf("%c",pem_array[(one << 4 & 0x30)+(two >> 4 & 0x0F)]);
		printf("=");
		printf("=");
	}else if(mod == 2){
		char one = str[index];
		char two = str[index+1];
                char three = 0;
		printf("%c",pem_array[one >> 2 & 0x3F]);
		printf("%c",pem_array[(one << 4 & 0x30)+(two >> 4 & 0x0F)]);
		printf("%c",pem_array[(two << 2 & 0x3C)+(three >> 6 & 0x3)]);
		printf("=");
	}
	
	printf("\n");



	//解码
	char *code = "MTIzNDU2Nzg5MA==";
	//TODO 去掉回车换行后需要4的倍数
        int code_length = 16;
	int inx = 0;

	
	while(inx < code_length){
		char one = getCharIndex(code[inx]);
		char two = getCharIndex(code[inx+1]);
		char three=getCharIndex(code[inx+2]);
		char four =getCharIndex(code[inx+3]);
		inx+=4;
		
		//0x3F 0011 1111
		//0x0F 0000 1111
		//0x03 0000 0011
		//0xFC 1111 1100
		//0xF0 1111 0000
		//0xC0 1100 0000
		//0x3F 0011 1111
		printf("%c",((one << 2 & 0xFC)+(two >> 4 & 0x03)));
		printf("%c",((two << 4 & 0xF0)+(three >> 2 & 0x0F)));
		printf("%c",((three << 6 & 0xC0)+(four & 0x3F)));
	}
	printf("\n");
}


static char getCharIndex(unsigned char c){
	if(c >= 'A' && c <= 'Z'){
		return c-'A';
	}
	
	if(c > 'a' && c <= 'z'){
		return c - 'a' + 26;
	}
	
	if(c >= '0' && c <= '9'){
		return c - '0' + 52;
	}
		
	if(c == '+'){
		return 62;
	}

	if(c == '/'){
		return 63;
	}

	if(c == '='){
		return 0;
	}

	return 0;
}



