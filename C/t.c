#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv){

	int size = getpagesize();

	printf("%d\n", size);

	if (0) {
		printf("%d\n", size);
	}

	char **a[1] = {"haha"};
	printf("%s\n",a[0]);
}
