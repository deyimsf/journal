#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv){

	int size = getpagesize();

	printf("%d\n", size);


}

