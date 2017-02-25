/*
*定义nginx内存池pool头文件
*/

/*如果没有定义下面这个宏则立即定义*/
#ifndef _NGX_PALLOC_H_INCLUDED_
#define _NGX_PALLOC_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>


/*
 * 可以从pool中分配的最大内存?
 * 其中ngx_pagesize的值来自文件
	src/os/unix/ngx_posix_init.c
   中的getpagesize()方法,返回内存分页大小
 */
#define NGX_MAX_ALLOC_FROM_POOL (ngx_pagesize - 1)

/*默认池大小; 这个大小的单位是什么  TODO*/
#define NGX_DEFAULT_POOL_SIZE	(16 * 1024)

/*分配内存时对齐位数*/
#define NGX_POOL_ALIGNMENT	16
#define NGX_MIN_POOL_SIZE	\
	ngx_align((sizeof(ngx_pool_t) + 2 * sizeof(ngx_pool_large_t)),	\
		NGX_POOL_ALIGNMENT)
/**
 *注:ngx_align(d,a)是一个宏,其意义就是把d变成a的倍数;也就是说
 * 最终返回的结果是按照a对齐的。
 */


typedef void(*ngx_pool_cleanup_pt)(void *data);

typedef struct ngx_pool_cleanup_s ngx_pool_cleanup_t;

struct ngx_pool_cleanup_s {
	ngx_pool_cleanup_pt	handler;
	void			*data;
	ngx_pool_cleanup_t	*next;
};


typedef struct ngx_pool_large_s	ngx_pool_large_t;

struct ngx_pool_large_s{
	ngx_pool_large_t	*next;
	void			*alloc;
};


typedef struct {
	u_char		*last; //最后一个未被使用的地址?
	u_char		*end;  //对于整个pool_data来说,它的最后?
	ngx_pool_t	*next;
	ngx_uint_t	failed;
} ngx_pool_data_t;


struct ngx_pool_s {
	ngx_pool_data_t		d;
	size_t			max;
	ngx_pool_t		*current;
	ngx_chain_t		*chain;
	ngx_pool_large_t	*large;
	ngx_pool_cleanup_t	*cleanup;
	ngx_log_t		*log;	
};


typedef struct {
	ngx_fd_t	fd;
	u_char		*name;
	ngx_log_t	*log;
} ngx_pool_cleanup_file_t;


void *ngx_alloc(size_t size, ngx_log_t *log);
void *ngx_calloc(size_t size, ngx_log_t);

ngx_pool_t *ngx_create_pool(size_t size, ngx_log_t *log);
void ngx_destroy_pool(ngx_pool_t *pool);
void ngx_reset_pool(ngx_pool_t *pool);

void *ngx_palloc(ngx_pool_t *pool, size_t size);
void *ngx_pnalloc(ngx_pool_t *pool, size_t size);
void *ngx_pcalloc(ngx_pool_t *pool, size_t size);
void *ngx_pmemalign(ngx_pool_t *pool, size_t size, size_t alignment);
ngx_int_t ngx_pfree(ngx_pool_t *pool, void *p);


ngx_pool_cleanup_t *ngx_pool_cleanup_add(ngx_pool_t *p, size_t size);
void ngx_pool_run_cleanup_file(ngx_pool_t *p, ngx_fd_t fd);
void ngx_pool_cleanup_file(void *data);
void ngx_pool_delete_file(void *data);


#endif /* _NGX_PALLOC_H_INCLUDED_ */

