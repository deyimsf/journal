#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv){

	int a = 6;

    switch(a) {

		case 8:{
			printf("case8: %d\n",a);
            case 7:
        	printf("case7 \n");     
		}
		default:
			printf("default: %d\n",a);
	
		case 6:
            printf("case6 \n");
	
	}



}

