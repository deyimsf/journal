#include <sys/stat.h>
#include <stdio.h>

int main(){
	struct stat buf;
	int f = stat("/etc/hosts", &buf);
	f = fstat(3, &buf);	

	printf("/etc/hosts file size = %lld\n", buf.st_size);

}
