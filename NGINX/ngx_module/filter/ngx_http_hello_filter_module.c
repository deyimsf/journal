//nginx filter 模块编写 

#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>


//模块保存模块指令结构
typedef struct {
	ngx_str_t	add_hello;
	
	ngx_hash_t	types;
	ngx_array_t	*types_keys;
} ngx_http_add_hello_loc_conf_t;

//自定义一个模块上下文
typedef struct {

} ngx_http_add_hello_ctx_t;

//指令
static ngx_command_t ngx_http_add_hello_commands[]={
	{ ngx_string("add_hello"),
	  NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
	  ngx_conf_set_str_slot,
	  NGX_HTTP_LOC_CONF_OFFSET,
	  offsetof(ngx_http_add_hello_loc_conf_t,add_hello),
	  NULL
	},

	ngx_null_command
}

//上下文
static ngx_http_module_t ngx_http_add_hello_filter_module_ctx = {
	NULL,
	ngx_http_add_hello_init,

	NULL,
	NULL,
	
	NULL,
	NULL,

	ngx_http_add_hello_create_conf,
	ngx_http_add_hello_merge_conf
}

//模块信息
ngx_module_t ngx_http_add_hello_filter_module = {
	NGX_MODULE_V1,
	$ngx_http_add_hello_filter_module_ctx,
	ngx_http_add_hello_commands,
	NGX_HTTP_MODULE,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NGX_MODULE_V1_PADDING
}

//全局变量,过滤头的方法和过滤体的方法
static ngx_http_output_header_filter_pt	ngx_http_next_header_filter;
static ngx_http_output_body_filter_pt	ngx_http_next_body_filter;

//注册过滤器
static ngx_int_t
ngx_http_add_hello_init(ngx_conf_t *cf)
{
	ngx_http_next_header_filter = ngx_http_top_header_filter;
	ngx_http_top_header_filter = ngx_http_add_hello_header_filter;

	ngx_http_next_body_filter = ngx_http_top_body_filter;
	ngx_http_top_body_filter = ngx_http_add_hello_body_filter;	
}

//创建本地变量
static void *
ngx_http_add_hello_create_conf(ngx_conf_t *cf)
{
	ngx_http_add_hello_conf_t *conf;	
	
	conf = ngx_pcalloc(cf->pool,sizeof(ngx_http_add_hello_conf_t));
    if(conf == NULL){
		return NULL;	
	}	
			
	return conf;
}

//merge指令值
static char *
ngx_http_add_hello_merge_conf(ngx_conf_t *cf, void *parent, void *child)
{
	ngx_http_add_hello_loc_conf_t *prev = parent;
	ngx_http_add_hello_loc_conf_t *next = child;

    ngx_conf_merge_str_value(conf->add_hello, prev->add_hello, " defalut");	
	
	return NGX_CONF_OK;
}

//过滤头
static ngx_int_t
ngx_http_add_hello_header_filter(ngx_http_request_t *r)
{
	ngx_http_add_hello_ctx_t *ctx;
	ngx_http_add_hello_conf_t *conf;

	//拿出loc_conf
	conf = ngx_http_get_module_loc_conf(r, ngx_http_add_hello_filter_module);
	
	if(conf->add_hello == NULL){
		return ngx_http_next_header_filter(r);	
	}			

	ngx_http_set_ctx(r, ctx, ngx_http_add_hello_filter_module);	
	
	ngx_http_clear_content_length(r); //?后面谁去指定长度
	ngx_http_clear_accept_ranges(r); //?干什么的
	
    return ngx_http_next_header_filter(r);
}

//过滤头
static ngx_int_t
ngx_http_add_hello_body_filter(ngx_http_request_t *r, ngx_chain_t *in)
{
	ngx_buf_t		*buf;
	ngx_chain_t		*nct;
	ngx_http_add_hello_ctx_t *ctx;

	//获取在header中保存的ctx
	ctx = ngx_http_get_module_ctx(r,ngx_http_add_hello_filter_module);
    if(ctx == NULL){ //如果header没有设置
		return ngx_http_next_body_filter(r,in);
	}	
	
	//为当前filter创建buf和chain_t
	buf = ngx_calloc_buf(r->pool);	
	if(buf == NULL){
		return NGX_ERROR;
	}

	nct = ngx_alloc_chain_link(r->pool);
	if(nct == NULL) {
		return NGX_ERROR;		
	}
		
    //为当前filter设置buf
	buf->pos = ctx->add_hello.data;
	buf->last = buf->pos + ctx->add_hello.len;
	buf->start = buf->pos;
	buf->end = buf->last;
	buf->last_buf = 1;
	buf->memory = 1;
	
	//将新建的chain_t放入输出链中
	ngx_chain_t *tmp;
	for(tmp = in; tmp; tmp = tmp->next){
		if(tmp->buf->last_buf){
			//是最后一buf
			tmp->buf->last_buf = 0;
			break;
		}
	}
	
	tmp.next = nct;
	nct->next = NULL;
	nct->buf = buf;	
		
	return ngx_http_next_body_filter(r,in);
}



