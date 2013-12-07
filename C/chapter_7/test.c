#include <stdio.h>
#include <stdlib.h>

main(int argc, char *argv){
	
	int *ip;
	ip  = (int *) calloc(1,sizeof(int));
	*ip = 9;	
	*(ip+1) = 8;	

	printf("%d %d\n",sizeof ip, *ip);
	printf("%d %d\n",sizeof ip, *(ip+1));
	printf("%d %d\n",sizeof ip, *(ip+2));
	
	printf("%d %x\n",sizeof ip, ip);
	printf("%d %x\n",sizeof ip, ip+1);
	printf("%d %x\n",sizeof ip, ip+2);


	//printf("%d %x %x %x\n",sizeof ip, &(*ip),&(*++ip),&(*++ip));
}
