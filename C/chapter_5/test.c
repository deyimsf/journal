#include <stdio.h>
#include <stdlib.h>

int numcmp(char *s1, char *s2){
	double v1,v2;
	v1 = atof(s1);
	v2 = atof(s2);
	printf("s1-%c\n",*s1);
	printf("v1-%g v2-%g\n",v1,v2);
}

main(){
	//numcmp("11","22");
	
	int (*daytab)[13];
	int bb[13];
	int *c;
	int aa[22][13];
	
	c = bb;
	daytab = &bb;
	daytab = aa;
	
	printf("%x  %x\n",c,daytab);

}
