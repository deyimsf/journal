//nginx filter 模块编写 

#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

static ngx_int_t ngx_http_subrequest_init(ngx_conf_t *cf);
static void *ngx_http_subrequest_create_loc_conf(ngx_conf_t *cf);
static char *ngx_http_subrequest_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child);
static ngx_int_t ngx_http_subrequest_header_filter(ngx_http_request_t *r);
static ngx_int_t ngx_http_subrequest_body_filter(ngx_http_request_t *r, ngx_chain_t *in);

//保存模块指令结构
typedef struct {
	ngx_str_t	sub_before;
	ngx_str_t	sub_after;	
} ngx_http_subrequest_loc_conf_t;

//自定义一个模块上下文
typedef struct {
	ngx_uint_t	sub_before_sent;
} ngx_http_subrequest_ctx_t;

//指令
static ngx_command_t ngx_http_subrequest_commands[]={
	{ ngx_string("sub_before"),
	  NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
	  ngx_conf_set_str_slot,
	  NGX_HTTP_LOC_CONF_OFFSET,
	  offsetof(ngx_http_subrequest_loc_conf_t,sub_before),
	  NULL
	},
	{ ngx_string("sub_after"),
	  NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
	  ngx_conf_set_str_slot,
	  NGX_HTTP_LOC_CONF_OFFSET,
	  offsetof(ngx_http_subrequest_loc_conf,sub_after),
	  NULL
	},

	ngx_null_command
};

//上下文
static ngx_http_module_t ngx_http_subrequest_filter_module_ctx={
	NULL,
	ngx_http_subrequest_init,

	NULL,
	NULL,
	
	NULL,
	NULL,

	ngx_http_subrequest_create_loc_conf,
	ngx_http_subrequest_merge_loc_conf
};

//模块信息
ngx_module_t ngx_http_subrequest_filter_module = {
	NGX_MODULE_V1,
	&ngx_http_subrequest_filter_module_ctx,
	ngx_http_subrequest_commands,
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
	ngx_http_top_header_filter = ngx_http_subrequest_header_filter;

	ngx_http_next_body_filter = ngx_http_top_body_filter;
	ngx_http_top_body_filter = ngx_http_subrequest_body_filter;	
	
	return NGX_OK;
}

//创建本地变量
static void *
ngx_http_subrequest_create_loc_conf(ngx_conf_t *cf)
{
	ngx_http_subrequest_loc_conf_t *conf;	
	
	conf = ngx_pcalloc(cf->pool,sizeof(ngx_http_subrequest_loc_conf_t));
    if(conf == NULL){
		return NULL;	
	}	
			
	return conf;
}

//merge指令值
static char *
ngx_http_subrequest_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child)
{
	ngx_http_subrequest_loc_conf_t *prev = parent;
	ngx_http_subrequest_loc_conf_t *conf = child;

    ngx_conf_merge_str_value(conf->sub_before, prev->sub_before,"");	
	ngx_conf_merge_str_value(conf->sub_after, prev->sub_after,"");
	return NGX_CONF_OK;
}

//过滤头
static ngx_int_t
ngx_http_subrequest_header_filter(ngx_http_request_t *r)
{
	ngx_http_subrequest_ctx_t *ctx;
	ngx_http_subrequest_loc_conf_t *conf;

	//如果响应状态码不是200,或者不是主请求,就不过滤响应头了
	if (r->headers_out.status != NGX_HTTP_OK || r != r->main){
	    return ngx_http_next_header_filter(r);
	}

	//拿出loc_conf
	conf = ngx_http_get_module_loc_conf(r, ngx_http_subrequest_filter_module);
	
	if(conf->sub_before.len == 0 || conf->sub_after.len == 0){
		return ngx_http_next_header_filter(r);	
	}			

	ctx = ngx_pcalloc(r->pool, sizeof(ngx_http_subrequest_ctx_t));
	ngx_http_set_ctx(r, ctx, ngx_http_subrequest_filter_module);	
	
    return ngx_http_next_header_filter(r);
}

//过滤体
static ngx_int_t
ngx_http_subrequest_body_filter(ngx_http_request_t *r, ngx_chain_t *in)
{
	ngx_buf_t			*buf;
	ngx_chain_t			*nct;
	ngx_http_subrequest_ctx_t 	*ctx;
	ngx_http_subrequest_loc_conf_t 	*conf;
	ngx_http_request_t	*psr;	//子请求
	ngx_int_t			rc;

	//如果要输出的链是NULL,或者客户端只要求输出响应头,则不在执行过滤逻辑
	if (in == NULL || r->header_only) {
	    return ngx_http_next_body_filter(r, in);
	}

	// 获取在header中保存的ctx
	ctx = ngx_http_get_module_ctx(r,ngx_http_subrequest_filter_module);
	// 如果header没有设置,则说明没有任何指令被配置
   	if(ctx == NULL){ 
		return ngx_http_next_body_filter(r,in);
	}
	
	conf = ngx_http_get_module_loc_conf(r,ngx_http_subrequest_filter_module);	

	// 发出before子请求
	if (!ctx->sub_before_sent) {
		// 设置已发送标记
		ctx->sub_before_sent = 1;

		// 如果存在该指令则发起子请求
		if (conf->sub_before.len) {
			if (ngx_http_subrequest(r,&conf->sub_before,NULL,&psr,NULL,0) != NGX_OK) {
				return NGX_ERROR;
			}	
		}
	}

	// 如果不存在sub_after指令,则执行下一个过滤器
	if (conf->sub_after.len == 0) {
		return ngx_http_next_body_filter(r,in);	
	}


	// 找出最后一个chain_t
	int last = 0;
    ngx_chain_t *tmp;
    for(tmp = in; tmp; tmp = tmp->next){
        if(tmp->buf->last_buf){
            // 有最后一buf
			// ? 在nginx中,第三个要执行的过滤器是ngx_http_copy_filter_module
			// 这个过滤器会把所有要输出的chain进行拷贝输出,并且每次只会拷贝部分数据
			// 很有可能我们在content阶段产生的很大的chain节点会被分割成多个chian
			// 所以不是每次都能看到最后一个buf的,一般情况如果能看到最后一个buf
			// 那么该过滤器应该就是最后一次执行了? 
			last = 1;
            break;
        }   
    }   

	rc = ngx_http_next_body_filter(r,in);

	// 如果输出发生错误或者当前还没有最后一个buf,则直接返回rc
	if (rc == NGX_ERROR || !last) {
		return rc;	
	}
	
    // 发起sub_after子请求
	if (ngx_http_subrequest(r,&conf->sub_after,NULL,&psr,NULL,0) != NGX_OK) {
		return NGX_ERROR;	
	}	
		
	return rc;
}

