#include <stdio.h>
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_palloc.h>


int main(){
	printf("sdfdfds\n");

	ngx_pool_t *pool = NULL;
	pool = ngx_create_pool(1024, NULL);
	
	printf("%p\n",pool);
}
