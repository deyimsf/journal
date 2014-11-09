#include <stdio.h>

void decode_utf_16le(char *cc,int length);

int main(int argc ,char **argv){
	//utf-16le编码			unidoce值 
	//朱: 0x3167			6731
	//, : 0x2C00			2C
	//聿：0x7F80			807F
	//  : 0x69D8A5DE		2A6A5	
	char cc[10] = {'\x31','\x67','\x2C','\x00','\x7F','\x80','\x69','\xD8','\xA5','\xDE'};
	decode_utf_16le(cc,10);
}


/*
 *解码utf_16le并打印
 */
void decode_utf_16le(char *cc,int length){
	int index = 0;
	while(index < length){
		//0xD800  1101 1000 0000 0000
		//0xDFFF  1101 1111 1111 1111
		//0xDC00  1101 1100 0000 0000
		unsigned short w1 = 0;
		unsigned short w2 = 0;
		if((index+1)>=length){
			printf("2数组越界");
			return;
		}

        short w1_h = cc[index+1]; //高8位
 		short w1_l = cc[index]; //低8位
		w1 = ((w1_h << 8 & 0xFF00)+(w1_l & 0xFF));

		//两个字节表示一个字符
		if(w1 < 0xD800 || w1 > 0xDFFF){
			printf("%04X\n",w1 & 0xFFFF);
			
			index +=2;
			continue;
		}

		//四个字节表示一个字符
		if(!(w1 > 0xD800 && w1 < 0xDFFF)){
			//非法字符
			printf("%04X\n",w1 & 0xFFFF);
			index += 2; //越过该非法字符
			return;
		}

		if((index+3) >= length){
			printf("4数组越界");
			return;
		}
		
		short w2_h = cc[index+3]; //高8位
		short w2_l = cc[index+2]; //低8位
		w2 = (w2_h << 8 & 0xFF00) + (w2_l & 0xFF);
		if(!(w2 > 0xDC00 && w2 < 0xDFFF)){
			//非法字符
			printf("%04X\n",w1 & 0xFFFF);
			index += 4; //越过该非法字符
			return;
		}
		
		w1 = w1 & 0x3FF; //取10位
		w2 = w2 & 0x3FF; //取10位
		long u = (w1 << 10) + w2 + 0x10000; //unicode

		printf("%08X\n",u & 0xFFFFFFFF);
		index += 4;
	}
}
