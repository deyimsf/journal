//nginx filter 模块编写 

#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

static ngx_int_t ngx_http_add_hello_init(ngx_conf_t *cf);
static void *ngx_http_add_hello_create_loc_conf(ngx_conf_t *cf);
static char *ngx_http_add_hello_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child);
static ngx_int_t ngx_http_add_hello_header_filter(ngx_http_request_t *r);
static ngx_int_t ngx_http_add_hello_body_filter(ngx_http_request_t *r, ngx_chain_t *in);

//保存模块指令结构
typedef struct {
	ngx_str_t	add_hello;
	
	//?这俩指令有啥用
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
};

//上下文
static ngx_http_module_t ngx_http_add_hello_filter_module_ctx={
	NULL,
	ngx_http_add_hello_init,

	NULL,
	NULL,
	
	NULL,
	NULL,

	ngx_http_add_hello_create_loc_conf,
	ngx_http_add_hello_merge_loc_conf
};

//模块信息
ngx_module_t ngx_http_add_hello_filter_module = {
	NGX_MODULE_V1,
	&ngx_http_add_hello_filter_module_ctx,
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
};

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
	
	return NGX_OK;
}

//创建本地变量
static void *
ngx_http_add_hello_create_loc_conf(ngx_conf_t *cf)
{
	ngx_http_add_hello_loc_conf_t *conf;	
	
	conf = ngx_pcalloc(cf->pool,sizeof(ngx_http_add_hello_loc_conf_t));
    if(conf == NULL){
		return NULL;	
	}	
			
	return conf;
}

//merge指令值
static char *
ngx_http_add_hello_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child)
{
	ngx_http_add_hello_loc_conf_t *prev = parent;
	ngx_http_add_hello_loc_conf_t *conf = child;

    ngx_conf_merge_str_value(conf->add_hello, prev->add_hello, " defalut");	
	
	return NGX_CONF_OK;
}

//过滤头
static ngx_int_t
ngx_http_add_hello_header_filter(ngx_http_request_t *r)
{
	ngx_http_add_hello_ctx_t *ctx;
	ngx_http_add_hello_loc_conf_t *conf;

	//如果响应状态码不是200,或者不是主请求,就不过滤响应头了
	if (r->headers_out.status != NGX_HTTP_OK || r != r->main){
	    return ngx_http_next_header_filter(r);
	}

	//拿出loc_conf
	conf = ngx_http_get_module_loc_conf(r, ngx_http_add_hello_filter_module);
	
	if(conf->add_hello.len == 0){
		return ngx_http_next_header_filter(r);	
	}			

	ctx = ngx_pcalloc(r->pool, sizeof(ngx_http_add_hello_ctx_t));
	ngx_http_set_ctx(r, ctx, ngx_http_add_hello_filter_module);	
	
	//如果无法响应内容长度则清除content_length头,nginx后续使用chunked编码
	ngx_http_clear_content_length(r);
	ngx_http_clear_accept_ranges(r); //?干什么的
	
    return ngx_http_next_header_filter(r);
}

//过滤体
static ngx_int_t
ngx_http_add_hello_body_filter(ngx_http_request_t *r, ngx_chain_t *in)
{
	ngx_buf_t			*buf;
	ngx_chain_t			*nct;
	ngx_http_add_hello_ctx_t 	*ctx;
	ngx_http_add_hello_loc_conf_t 	*conf;

	//如果要输出的链是NULL,或者客户端只要求输出响应头,则不在执行过滤逻辑
	if (in == NULL || r->header_only) {
	    return ngx_http_next_body_filter(r, in);
	}

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
	
	conf = ngx_http_get_module_loc_conf(r,ngx_http_add_hello_filter_module);	
    	
	//为当前filter设置buf
	//增加一个换行
	u_char* tmp_buf = ngx_pcalloc(r->pool, conf->add_hello.len + 5);
	ngx_sprintf(tmp_buf,"%s\n",conf->add_hello.data);
	
	buf->pos = tmp_buf;	//conf->add_hello.data;
	buf->last = buf->pos + ngx_strlen(tmp_buf); //conf->add_hello.len;
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
	
	tmp->next = nct;
	nct->next = NULL;
	nct->buf = buf;	
		
	return ngx_http_next_body_filter(r,in);
}



