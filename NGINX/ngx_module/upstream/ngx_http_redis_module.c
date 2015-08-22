#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

/*
 该模块只处理redis的get命令

 location /redis {
	set $redis_get name;
	redis_pass 127.0.0.1:6379;
 }
*/

//loc_conf数据结构
typedef struct {
	ngx_http_upstream_conf_t	upstream_conf; //必须有一个?
	ngx_int_t					index; //用来存放redis key
} ngx_http_redis_loc_conf_t;

//redis模块自定义结构
typedef struct {

} ngx_http_redis_ctx_t;

//upstream模块的一些回调函数
static ngx_int_t ngx_http_redis_create_request(ngx_http_request_t *r);
static ngx_int_t ngx_http_redis_reinit_request(ngx_http_request_t *r);
static ngx_int_t ngx_http_redis_process_header(ngx_http_request_t *r);
static ngx_int_t ngx_http_redis_filter_init(void *data);
static ngx_int_t ngx_http_redis_filter(void *data, ssize_t bytes);
static void ngx_http_redis_abort_request(ngx_http_request_t *r);
static void ngx_http_redis_finalize_request(ngx_http_request_t *r, ngx_int_t rc);

//身为handlerm模块的一些回调函数
static void *ngx_http_redis_create_loc_conf(ngx_conf_t *cf);
static char *ngx_http_redis_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child);

//处理redis_pass的命令函数,在这里会对ngx_http_redis_handler函数进行按需挂载
static char *ngx_http_redis_pass(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

//handler函数,upstream会在这里面注册各种回调函数
static ngx_int_t ngx_http_redis_handler(ngx_http_request_t *r);




//模块的命令
static ngx_command_t ngx_http_redis_commands[] = {
	{ ngx_string("redis_pass");
	  NGX_HTTP_LOC_CONF|NGX_HTTP_LIF_CONF|NGX_CONF_TAKE1,
	  ngx_http_redis_pass,
	  NGX_HTTP_LOC_CONF_OFFSET,
	  0,
	  NULL},
	
	  ngx_null_command
};

//模块上下文
static ngx_http_module_t ngx_http_redis_module_ctx = {
	NULL,								/* preconfiguration */
	NULL,								/* postconfiguration */
	
	NULL,								/* create main configuration */
	NULL,								/* init main configuration */

	NULL,								/* create server configuration */
	NULL,								/* merge server configuration */
	
	ngx_http_redis_create_loc_conf,		/* create location configuration */
	ngx_http_redis_merge_loc_conf		/* merge location configuration */
};

//模块定义
ngx_module_t  ngx_http_memcached_module = { 
    NGX_MODULE_V1,
    &ngx_http_redis_module_ctx,        	   /* module context */
    ngx_http_redis_commands,           	   /* module directives */
    NGX_HTTP_MODULE,                       /* module type */
    NULL,                                  /* init master */
    NULL,                                  /* init module */
    NULL,                                  /* init process */
    NULL,                                  /* init thread */
    NULL,                                  /* exit thread */
    NULL,                                  /* exit process */
    NULL,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};

//创建loc_conf结构体
static void *ngx_http_redis_create_loc_conf(ngx_conf_t *cf){
	ngx_http_redis_loc_conf_t	*conf;
	
	conf = ngx_pcalloc(cf->pool, sizeof(ngx_http_redis_loc_conf_t));
	if (conf == NULL) {
		return NULL;
	}

	return conf;
}

//merge
static char *
ngx_http_redis_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child)
{
	ngx_http_redis_loc_conf_t *prev = parent;
	ngx_http_redis_loc_conf_t *conf = child;

	//TODO
	
	return NGX_CONF_OK;
}

//解析redis_pass指令的函数
//我们在这个函数中,对该模块进行按需挂载
static char *
ngx_http_redis_pass(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
	ngx_http_redis_loc_conf_t *mlcf = conf;
	
	ngx_str_t					*value; //用来指向redis_pass指令参数的指针
	ngx_url_t				 	 u;		//url
	ngx_http_core_loc_conf_t	*clcf;

	//?为什么判断这个
	if (mlcf->upstream_conf.upstream) {
		return "is duplicate";
	}

	value = cf->args->elts; //redis_pass指令的参数,args是ngx_array_t指针,elts是ngx_str_t指针

	// 初始化url
	ngx_memzero(&u, sizeof(ngx_url_t));
	
	// 赋值
	u.url = value[1]; //将redis_pass的第一个参数赋值给u.url
	u.no_resolve = 1; //?

	// 创建upstream吗?????
	mlcf->upstream_conf.upstream = ngx_http_upstream_add(cf, &u, 0);
	if (mlcf->upstream_conf.upstream == NULL) {
		return NGX_CONF_ERROR;
	}
	
	// 按需挂载
	clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
	// 在handler里面会注册upstream的各种回调函数
	clcf->handler = ngx_http_redis_handler;

	// 获取redis_get变量的索引并保存
	mlcf->index = ngx_http_get_variable_index(cf, &ngx_http__key);
	
	if (mlcf->index == NGX_ERROR){
		return NGX_CONF_ERROR;
	}

	return NGX_CONF_OK;
}

//在handler函数中,注册upstream的各种回调函数
static ngx_int_t
ngx_http_redis_handler(ngx_http_request_t *r)
{
	ngx_int_t						 rc;
	ngx_http_upstream_t				*u
	ngx_http_redis_ctx_t			*ctx;
	ngx_http_redis_loc_conf_t		*mlcf;

	//TODO 做一些方法合法性的判断,比如支持NGX_HTTP_GET操作
	
	//1.创建upstream数据结构
	if(ngx_http_upstream_create(r) != NGX_OK){
		return NGX_HTTP_INTERNAL_SERVER_ERROR;	
	}

	//2.设置模块的tag和schema。 ?有啥用?
	u = r->upstream; //步骤1中创建的结构. ?loc_conf中的upstream_conf是干什么的?	
    ngx_str_set($u->schema,"redis://");	
	u->output.tag = (ngx_buf_tag_t)&ngx_http_redis_module;

	//3.设置upstream的后端服务器列表结构?
	mlcf = ngx_http_get_module_loc_conf(r,ngx_http_redis_module);
	u->conf = &mlcf->upstream_conf; //应该就是upstream的一些配置信息

	//4.设置回调函数
	//TODO 明天搞

	
}




