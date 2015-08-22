//参考rds-json-nginx-module-0.13

#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

static ngx_int_t ngx_http_hello_init(ngx_conf_t *cf);
static void *ngx_http_hello_create_loc_conf(ngx_conf_t *cf);
static char *ngx_http_hello_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child);
static char * ngx_http_hello_string(ngx_conf_t *cf,ngx_command_t *cmd,void *conf);
static ngx_int_t ngx_http_hello_handler(ngx_http_request_t *r);


//存储location区域的配置信息
typedef struct{ 
	ngx_str_t hello_string;
	ngx_str_t hello_test;
	ngx_int_t index;
} ngx_http_hello_loc_conf_t;

//模块的指令信息
static ngx_command_t ngx_http_hello_commands[]={
	{
		ngx_string("hello_string"), 
		NGX_HTTP_LOC_CONF|NGX_CONF_NOARGS|NGX_CONF_TAKE1, //loc区域，一个或零个参数
		ngx_http_hello_string, //解析该指令的函数,在这里可以把指令信息赋值到我们定义上面定义的结构中 
		//用loc区域的pool分配内存,也就是解析后的数据放入哪里
		//如果某个指令可以用在main、server配置部分，但不能用在location部分,则应该使用srv_conf存储
		//存loc_conf中有冗余,存在man_conf中有覆盖
		NGX_HTTP_LOC_CONF_OFFSET,	
        //?猜测用到ngx的赋值函数如ngx_conf_set_str_slot的时候,会用到这个字段
        offsetof(ngx_http_hello_loc_conf_t,hello_string), 
		NULL
	},
	ngx_null_command
};

//模块上下文
static ngx_http_module_t ngx_http_hello_module_ctx={
	NULL,				 /*preconfiguration*/
	ngx_http_hello_init, /*postconfiguration*///创建和读取该模块的配置信息之后调用
	
	NULL,				 /*create main configuration*/
	NULL,				 /*init main configuration*/

	NULL,				 /*create server configuration*/
	NULL,				 /*merge server configuration*/

						 //创建自定义的结构体,用来存放指令参数
	ngx_http_hello_create_loc_conf,  /*create location configuration*/
	NULL				 /*merge location configuration*/
};

//模块的定义
ngx_module_t ngx_http_hello_module = {
	NGX_MODULE_V1,			/*版本号*/
	&ngx_http_hello_module_ctx,	/*模块上下文结构,可使用里面的一些函数*/
	ngx_http_hello_commands,	/*模块定义的指令*/
	NGX_HTTP_MODULE,		/*module type*/
	NULL,				/*init master*/ master进程初始化时执行
	NULL,				/*init module*/ master进程解析配置以后,初始化模块时调用一次
	NULL,				/*init process*/worker进程初始化时调用一次
	NULL,				/*init thread*/ 多线程时,线程初始化时调用,Linux下未使用
	NULL,				/*exit thread*/ 多线程退出是调用一次
	NULL,				/*exit process*/worker进程退出时调用一次
	NULL,				/*exit master*/ master进程退出时调用一次
	NGX_MODULE_V1_PADDING
};


//上下文中postconfiguration的函数
static ngx_int_t ngx_http_hello_init(ngx_conf_t *cf){
	ngx_http_handler_pt		*h;  //真正处理请求的函数
	ngx_http_core_main_conf_t	*cmcf;
	
	cmcf = ngx_http_conf_get_module_main_conf(cf,ngx_http_core_module);
    //另一种挂载方式,按需挂载
    //目前只知道在指令方法里面按需才能挂载上，为什么?
    //clcf = ngx_http_conf_get_module_loc_conf(cf,ngx_http_core_module);
	//clcf->handler = ngx_http_hello_handler;

	//函数注册到CONTENT阶段
	h = ngx_array_push(&cmcf->phases[NGX_HTTP_CONTENT_PHASE].handlers);
	if(h == NULL){
		return NGX_ERROR;
	}

	*h = ngx_http_hello_handler;  //真正处理请求的函数
	
	return NGX_OK;
}

// 创建loc域的结构体
static void *ngx_http_hello_create_loc_conf(ngx_conf_t *cf){
	ngx_http_hello_loc_conf_t *conf = NULL;
	conf  = ngx_pcalloc(cf->pool,sizeof(ngx_http_hello_loc_conf_t));
	if (conf == NULL){
		return NULL;
	}

    //初始化结构体
	ngx_str_null(&(conf->hello_string));
	ngx_str_set(&(conf->hello_test),"hello world test");

	return conf;
}

// 合并loc域的配置信息
static char *ngx_http_hello_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child){

	ngx_http_hello_loc_conf_t *prev = parent;
	ngx_http_hello_loc_conf_t *conf = child;
	
	ngx_conf_merge_str_value(conf->hello_string,prev->hello_string,"defalut");	

	return NGX_CONF_OK;
}

//处理解析到的指令
static char * ngx_http_hello_string(ngx_conf_t *cf,ngx_command_t *cmd,void *conf){
	ngx_http_hello_loc_conf_t * loc_conf;
	loc_conf = (ngx_http_hello_loc_conf_t *)conf;

	//赋值解析到的值
    char *rv = ngx_conf_set_str_slot(cf,cmd,loc_conf);
	//ngx_str_set(conf->hello_string,"mashunfeng");
	

	//检查是否有变量
	ngx_str_t t = loc_conf->hello_string;
	if (t.data[0] == '$') {
		t.data++;
		t.len--;
	
		//索引这个变量,并将其保存到loc_conf中
		ngx_uint_t index = ngx_http_get_variable_index(cf,&t);	
		loc_conf->index = index;
	}

	return NGX_CONF_OK;
}

//真正处理请求的函数
static ngx_int_t ngx_http_hello_handler(ngx_http_request_t *r){
	ngx_int_t rc;
	ngx_buf_t *b;
	ngx_chain_t out;
	ngx_http_hello_loc_conf_t *my_conf;

	//存放从my_conf中都到的信息
	ngx_uint_t content_length = 0;

	//取出我们的指令配置信息
	my_conf = ngx_http_get_module_loc_conf(r,ngx_http_hello_module);
    //如果指令不存在,则放弃执行该方法	
    if(my_conf->hello_string.len == 0){ 
	   return NGX_DECLINED;
    }

	//如果hello_string指令参数是变量
	ngx_str_t t = my_conf->hello_string;
	if(t.data[0] == '$'){
		t.data++;
		t.len--;
	
		ngx_http_variable_value_t *value;
		/*如果变量无法索引,可以用下面这种方式获取变量
		  ngx_uint_t hash;
		  hash = ngx_hash_strlow(t.data,t.data,t.len);
		  value = ngx_http_get_variable(r,&t,hash);
		*/
		value = ngx_http_get_indexed_variable(r,my_conf->index);
			
		t.data = value->data;
		t.len = value->len;
	}

	//将数据读入到临时buf中
	u_char* tmp_buf = ngx_pcalloc(r->pool, my_conf->hello_string.len + 1);// [1024] = {0};
	ngx_sprintf(tmp_buf,"%s",t.data);
	
	content_length = ngx_strlen(tmp_buf); 
    //如果我们不需要请求体,那就把它抛弃
	rc = ngx_http_discard_request_body(r);
	if(rc != NGX_OK){
		return rc;
	}

	//将数据添加到缓存链并输出
    b = ngx_pcalloc(r->pool,sizeof(ngx_buf_t));	
	if (b == NULL){
		return NGX_HTTP_INTERNAL_SERVER_ERROR;
	}
	
	out.buf = b;
	out.next = NULL;
	
	b->pos = tmp_buf;
	b->last = tmp_buf + content_length;
	
	//这个buffer在内存中,后续在write filter中用该标记判断从哪里输出数据
	//如果是 b->file = 1 则表示后续要输出的内容在file中,如果同时设置则file起作用
	b->memory = 1; 
	b->last_buf = 1; //这是buffer chain中的最后一个buffer 

	//需要设置响应头
	r->headers_out.status = NGX_HTTP_OK; 
	r->headers_out.content_length_n = content_length;
    	
	//需要发送响应头
	rc = ngx_http_send_header(r);
	if(rc == NGX_ERROR || rc > NGX_OK || r->header_only){
		return rc;
	}

	//把buffer chain输出到response
	return ngx_http_output_filter(r,&out);
}

