#对于handler两种挂载方式的疑问
  1.按阶段挂载
    ngx_http_handler_pt     *h;
    ngx_http_core_main_conf_t   *cmcf;    
    cmcf = ngx_http_conf_get_module_main_conf(cf,ngx_http_core_module);
    //函数注册到CONTENT阶段
    h = ngx_array_push(&cmcf->phases[NGX_HTTP_CONTENT_PHASE].handlers);
    *h = ngx_http_hello_handler;  //真正处理请求的函数
    
	对于这种挂载方式,当有一个空location的时候,仍然会执行ngx_http_hello_handler
    方法,如果不想上这个函数执行可以返回NGX_DECLINED,这时候就会去执行nginx默认
    的index、autoindex、static等模块
	location ~ ^/test {
			
    }


  2.另一种挂载方式,按需挂载
    ngx_http_core_loc_conf_t clcf;
	clcf = ngx_http_conf_get_module_loc_conf(cf,ngx_http_core_module);
    clcf->handler = ngx_http_hello_handler;

    ?疑问，为什么这种方式在postconfiguration方法中挂载不上，好像只能在具体的指令
    处理函数中才能挂载上.
    回答:
	首先,这种方式挂载的指令只有出现在location{}块才会起作用
	如果某个指令是用这种方式挂载的,那么指令只有出现在location{}块才有意义
	出现在http{}和server{}块是没有意义的,因为这种方式只有匹配到某个具体的location后才会去
	回调该location{}块所属的ngx_http_core_loc_conf_t结构体的handler方法

	最后，只有在解析到某个location下的hello_string指令后，才会拿到属于该location{}块
	的ngx_http_core_loc_conf_t结构体
 

    另外((ngx_http_conf_ctx_t *) cf->ctx)->loc_conf 变量在http{} server{} locaiton{} 
    和指令的处理函数中不一样



