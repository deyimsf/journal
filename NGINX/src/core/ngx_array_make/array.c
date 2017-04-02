#include <stdio.h>
#include <ngx_config.h>
#include <ngx_core.h>


int main(){
	ngx_pool_t *pool = ngx_create_pool(1024, NULL);
	
	ngx_array_t *array = ngx_array_create(pool,10,30);

	ngx_array_destroy(array);
	
	printf("end...\n");

	ngx_cacheline_size = 32;

	printf("ngx_cacheline_size:%lu\n",ngx_cacheline_size);
}
