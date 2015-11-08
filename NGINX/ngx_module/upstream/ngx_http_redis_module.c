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
	ngx_int_t			index; //用来存放redis key
} ngx_http_redis_loc_conf_t;

//redis模块自定义结构
typedef struct {
	//input_filter方法没有传入request参数,所以我们可以将
	//request放入定义上下文中,以便后续使用
	ngx_http_request_t			*request;
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
	{ ngx_string("redis_pass"),
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
ngx_module_t  ngx_http_redis_module = { 
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

	//初始化upstream的一些变量

	conf->upstream_conf.local = NGX_CONF_UNSET_PTR;
    conf->upstream_conf.next_upstream_tries = NGX_CONF_UNSET_UINT;
   	conf->upstream_conf.connect_timeout = NGX_CONF_UNSET_MSEC;
    conf->upstream_conf.send_timeout = NGX_CONF_UNSET_MSEC;
   	conf->upstream_conf.read_timeout = NGX_CONF_UNSET_MSEC;
    conf->upstream_conf.next_upstream_timeout = NGX_CONF_UNSET_MSEC;

    	/* the hardcoded values */
    	/*conf->upstream_conf.cyclic_temp_file = 0;
    	conf->upstream_conf.buffering = 0;
    	conf->upstream_conf.ignore_client_abort = 0;
    	conf->upstream_conf.send_lowat = 0;
    	conf->upstream_conf.bufs.num = 0;
    	conf->upstream_conf.busy_buffers_size = 0;
    	conf->upstream_conf.max_temp_file_size = 0;
    	conf->upstream_conf.temp_file_write_size = 0;
    	conf->upstream_conf.intercept_errors = 1;
    	conf->upstream_conf.intercept_404 = 1;
    	conf->upstream_conf.pass_request_headers = 0;
    	conf->upstream_conf.pass_request_body = 0;
	*/

	return conf;
}

//merge
static char *
ngx_http_redis_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child)
{
	ngx_http_redis_loc_conf_t *prev = parent;
	ngx_http_redis_loc_conf_t *conf = child;

	ngx_conf_merge_ptr_value(conf->upstream_conf.local,
                              prev->upstream_conf.local, NULL);

   	ngx_conf_merge_uint_value(conf->upstream_conf.next_upstream_tries,
                              prev->upstream_conf.next_upstream_tries, 0); 

	ngx_conf_merge_msec_value(conf->upstream_conf.connect_timeout,
                              prev->upstream_conf.connect_timeout, 60000);

	ngx_conf_merge_msec_value(conf->upstream_conf.send_timeout,
                              prev->upstream_conf.send_timeout, 60000);

  	ngx_conf_merge_msec_value(conf->upstream_conf.read_timeout,
                              prev->upstream_conf.read_timeout, 60000);

   	ngx_conf_merge_msec_value(conf->upstream_conf.next_upstream_timeout,
   	                           prev->upstream_conf.next_upstream_timeout, 0); 

	
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
	u.no_resolve = 1; //

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
	ngx_str_t ngx_http_redis_get = ngx_string("redis_get");
	mlcf->index = ngx_http_get_variable_index(cf, &ngx_http_redis_get);
	
	if (mlcf->index == NGX_ERROR){
		return NGX_CONF_ERROR;
	}

	return NGX_CONF_OK;
}

//在handler函数中,注册upstream的各种回调函数
static ngx_int_t
ngx_http_redis_handler(ngx_http_request_t *r)
{
	//ngx_int_t				rc;
	ngx_http_upstream_t			*u;
	ngx_http_redis_ctx_t			*ctx;
	ngx_http_redis_loc_conf_t		*mlcf;

	//TODO 做一些方法合法性的判断,比如支持NGX_HTTP_GET操作
	
	//1.创建upstream,之后r->upstream就不是NULL了
	if(ngx_http_upstream_create(r) != NGX_OK){
		return NGX_HTTP_INTERNAL_SERVER_ERROR;	
	}

	//2.设置模块的tag和schema。 ?有啥用?
	u = r->upstream; //步骤1中创建的结构. ?loc_conf中的upstream_conf是干什么的?	
    	ngx_str_set(&u->schema,"redis://");	
	u->output.tag = (ngx_buf_tag_t)&ngx_http_redis_module;

	//3.设置upstream的后端服务器列表结构?
	mlcf = ngx_http_get_module_loc_conf(r,ngx_http_redis_module);
	u->conf = &mlcf->upstream_conf; //应该就是upstream的一些配置信息

	//4.设置回调函数
	u->create_request = ngx_http_redis_create_request;
	u->reinit_request = ngx_http_redis_reinit_request;
	u->process_header = ngx_http_redis_process_header;
	u->abort_request = ngx_http_redis_abort_request;
	u->finalize_request = ngx_http_redis_finalize_request;
	u->input_filter_init = ngx_http_redis_filter_init;
	u->input_filter = ngx_http_redis_filter;
	
	//5.创建upstream环境数据结构,自定义上下文
 	ctx = ngx_palloc(r->pool, sizeof(ngx_http_redis_ctx_t));
	if (ctx == NULL) {
		return NGX_HTTP_INTERNAL_SERVER_ERROR;
	}	
	
	ctx->request = r; //把当前请求放入到自定义上下文中
	
	ngx_http_set_ctx(r, ctx, ngx_http_redis_module);

	u->input_filter_ctx = ctx;//调用input_filter时入参*data值为ctx

	//6.完成初始化并进行收尾工作
	r->main->count++;
	ngx_http_upstream_init(r);
	
	return NGX_DONE;
}

//创建并发送请求
static ngx_int_t
ngx_http_redis_create_request(ngx_http_request_t *r)
{
	size_t							len;
	ngx_buf_t						*b;
	ngx_chain_t						*cl;
	ngx_http_variable_value_t		*vv; //变量值
	ngx_http_redis_loc_conf_t		*mlcf;

	//1.获取变量值
	mlcf = ngx_http_get_module_loc_conf(r, ngx_http_redis_module);
	vv = ngx_http_get_indexed_variable(r, mlcf->index);

	if (vv == NULL || vv->not_found || vv->len == 0) {
		ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, 
						"the \"$redis_get\" variable is not set");
		return NGX_ERROR;
	}
	
	//2.创建存放请求命令的buf
	len = sizeof("get ") - 1 + vv->len + sizeof(CRLF) - 1;
	b = ngx_create_temp_buf(r->pool, len);
	if (b == NULL) {
		return NGX_ERROR;
	}

	//3.创建存放命令buf的chain
	cl = ngx_alloc_chain_link(r->pool);
	if (cl == NULL) {
		return NGX_ERROR;
	}	

	cl->buf = b;
	cl->next = NULL;
	
    //请求数据封装成chain赋值给upstream
	r->upstream->request_bufs = cl;

	//4.赋值
	u_char *tmp_buf = ngx_pcalloc(r->pool, len+1);
        /*u_char *tmp = tmp_buf;
	*tmp++ = 'g';
	*tmp++ = 'e';
	*tmp++ = 't';
	*tmp++ = ' ';
	*tmp++ = 'n';
	*tmp++ = 'a';
	*tmp++ = 'm';
	*tmp++ = 'e';
	*tmp++ = '\r';
	*tmp++ = '\n';
	*/
	ngx_sprintf(tmp_buf,"%s%s\r\n","get ",vv->data);

	b->pos = tmp_buf;
	b->last = tmp_buf+len;

	return NGX_OK;
}


//某个后端服务失败后要做的事
static ngx_int_t
ngx_http_redis_reinit_request(ngx_http_request_t *r)
{
	return NGX_OK;
}

//客户端放弃请求时调用? 什么叫放弃?
static void
ngx_http_redis_abort_request(ngx_http_request_t *r)
{
	ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
					"abort http redis request");
	return;	
}

//正常完成与后端服务器请求后调用该函数
static void
ngx_http_redis_finalize_request(ngx_http_request_t *r, ngx_int_t rc)
{
	ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
					"finalize http redis request");

	return;
}

//处理协议头
//如果协议头数据未完全返回,可以让该方法放回NGX_AGAIN
//当链路有数据时会重新调用该方法
static ngx_int_t
ngx_http_redis_process_header(ngx_http_request_t *r)
{
	ngx_http_upstream_t				*u;

	u = r->upstream;

	//在未解析完协议头之前,upstream返回的数据都会在u->buffer这一块缓存中
   
	//
	r->headers_out.content_length_n = 2;
	u->headers_in.status_n = 200; //?
	u->state->status = 200;
	
	//我们不处理协议头,直接返回
	return NGX_OK; 	
}

//处理服务器返回的响应正文
//data 存放在ngx_http_set_ctx(r,ctx,ngx_http_[name]_module)中的ctx
//bytes ??
//?会多次进来吗
static ngx_int_t
ngx_http_redis_filter(void *data, ssize_t bytes)
{
	/*
	ngx_http_redist_ctx_t	*ctx = data;
	
	u_char					*last;
	ngx_buf_t				*b;
	ngx_http_upstream_t 	*u;

	u = ctx->request->upstream;
	b = &u->buffer;
	*/

	//最终是把接收到的数据组成chain追加到u->out_bufs中

	return NGX_OK;	
}

//初始化input_filter
static ngx_int_t
ngx_http_redis_filter_init(void *data)
{
	ngx_http_redis_ctx_t		*ctx = data;
	ngx_http_upstream_t			*u;

	u = ctx->request->upstream;

	//TODO设置一些值
	//u->length = u->length;

	return NGX_OK;
}




