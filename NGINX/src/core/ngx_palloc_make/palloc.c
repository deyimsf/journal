#include <stdio.h>
#include <ngx_config.h>
#include <ngx_core.h>


int main(){
	ngx_pool_t *pool = NULL;
	pool = ngx_create_pool(79, NULL);
	
	printf("%lu\n",pool->max);
}
