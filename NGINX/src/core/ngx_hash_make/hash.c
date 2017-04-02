#include <stdio.h>
#include <ngx_config.h>
#include <ngx_core.h>

int main(){
	//数字对齐方法ngx_align用到了/os/unix/ngx_alloc.c中ngx_cacheline_size的值
	//如果不启动nginx则该值为0,这时ngx_align就一直返回为0,所以我们这给他一个默认值
	ngx_cacheline_size = 32;

	//用于构造带通配符的散列表
	ngx_hash_keys_arrays_t  ha;

	ngx_pool_t *pool = ngx_create_pool(2024, NULL);
	
	ha.temp_pool = pool;
	ha.pool = pool;

	if(ngx_hash_keys_array_init(&ha,NGX_HASH_LARGE) != NGX_OK){
		printf("初始化失败\n");	
	
		return 0;
	}

	
	//添加元素 *.jd.com
	ngx_str_t *key = ngx_pcalloc(pool, sizeof(ngx_str_t));
	key->len = ngx_strlen("*.com");
	key->data = ngx_pcalloc(pool, ngx_strlen("*.com"));
	ngx_memcpy(key->data, "*.com", ngx_strlen("*.com"));	

	int *value = ngx_pcalloc(pool, sizeof(int));
	*value = 1234; 

	ngx_int_t rc;
	rc = ngx_hash_add_key(&ha, key, value, NGX_HASH_WILDCARD_KEY);
        if(rc != NGX_OK){
		printf("添加key错误\n");
	}	

	//构造通配符散列表
	ngx_hash_init_t hash;
	hash.hash = NULL;
	hash.key = ngx_hash_key;
	hash.max_size = 10;
	hash.bucket_size = 100;
	hash.pool = pool;
	hash.temp_pool = pool;

	//初始化前置通配符散列表	
       	rc = ngx_hash_wildcard_init(&hash, ha.dns_wc_head.elts,
					ha.dns_wc_head.nelts);		

	if(rc != NGX_OK){
		printf("初始化通配符散列表错误\n");
	}
	
	// *.com和 as.dd.com a.b.c.com都匹配
	void *result = ngx_hash_find_wc_head((ngx_hash_wildcard_t *)hash.hash,
				"as.dd.com",ngx_strlen("as.dd.com"));
	
	int *bb = (int *)result;
	printf("%d\n",*bb);

}
