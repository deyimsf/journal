
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

typedef struct{ //保存模块的配置信息的结构
	ngx_str_t hello_string; //保存某个指令配置项的值
	ngx_int_t hello_counter;
} ngx_http_hello_loc_conf_t;

//模块的配置指令,定义指令名字、使用区域等
static ngx_command_t ngx_http_hello_commands[]={
	{
		ngx_string("hello_string"), //指令名字
		NGX_HTTP_LOC_CONF|NGX_CONF_NOARGS|NGX_CONF_TAKE1, //loc区域，一个或零个参数
		ngx_http_hello_string, //解析该指令的函数，返回字符指针(字符串) 
				       //char *(*set)(ngx_conf_t *cf,ngx_command_t *cmd, void *conf)
				       //比如校验指令配置是否正确，只有返回NGX_CONF_OK才会执行后续任务
		NGX_HTTP_LOC_CONF_OFFSET, //该配置项存储的内存位置
		offsetof(ngx_http_hello_loc_conf_t,hello_string), //该配置项的值，存放在结构的hello_string字段中
 				      				  //如果值不需要保存或需保存到更复杂结构,可设置0
		NULL
	},
	{
		ngx_string("hello_counter"),
		NGX_HTTP_LOC_CONF|NGX_CONF_FLAG,
		ngx_http_hello_counter,
		NGX_HTTP_LOC_CONF_OFFSET,
		offsetof(ngx_http_hello_loc_conf_t,hello_counter),
		NULL
	},
	ngx_null_command
};

//模块上下文结构，该结构里面都是函数指针，nginx会在合适的时间调用这些函数
static ngx_http_module_t ngx_http_hello_module_ctx={
	NULL,				/*preconfiguration*/
	ngx_http_hello_init,		/*postconfiguration*///创建和读取该模块的配置信息之后调用
	
	NULL,				/*create main configuration*/
	NULL,				/*init main configuration*/

	NULL,				/*create server configuration*/
	NULL,				/*merge server configuration*/

	ngx_http_hello_create_loc_conf  /*create location configuration*/
	NULL				/*merge location configuration*/
};

//模块的定义
//定义一个ngx_module_t类型的变量，该变量告诉nginx模块本身的信息
//该变量里面包含了模块上下文信息，模块定义的指令等
ngx_module_t ngx_http_hello_module = {
	NGX_MODULE_V1,			/*站位*/
	&ngx_http_hello_module_ctx,	/*模块上下文结构,可使用里面的一些函数*/
	ngx_http_hello_commands,	/*模块定义的指令*/
	NGX_HTTP_MODULE,		/*module type*/
	NULL,				/*init master*/
	NULL,				/*init module*/
	NULL,				/*init process*/
	NULL,				/*init thread*/
	NULL,				/*exit thread*/
	NULL,				/*exit process*/
	NULL,				/*exit master*/
	NGX_MODULE_V1_PADDING
};

//----接下来就是写需要的函数了
//例如真正处理来自客户端请求的handler函数
//该函数可以自己直接生成内容，页可以选择拒绝处理等,函数声明如下:
//typedef ngx_init_t (*ngx_http_handler_pt)(ngx_http_request_t *r)

//创建和读取该模块的配置信息之后调用该函数
//该函数已经设置到模块上下文结构中(ngx_http_hello_module_ctx)
//在该函数里面挂在处理请求的handler
static ngx_ini_t ngx_http_hello_init(ngx_con_t *cf){
	ngx_http_handler_pt		*h;
	ngx_http_core_main_conf_t	cmcf
	
	cmcf = ngx_http_conf_get_module_main_conf(cf,ngx_http_core_module);
	
	h = ngx_array_push(&cmcf->phases[NGX_HTTP_CONTENT_PHASE].handlers);
	if(h == NULL){
		return NGX_ERROR;
	}

	*h = ngx_http_hello_handler;  //真正处理请求的函数
	
	return NGX_OK;
}

//真正处理请求的函数
static ngx_ini_t ngx_http_hello_handler(ngx_http_request_t *r){
	ngx_int_t rc;
	ngx_buf_t *b;
	ngx_chain_t out;
	//TODO something
	
	return rc 或者 return ngx_http_output_filter(r,&out);
	
}




