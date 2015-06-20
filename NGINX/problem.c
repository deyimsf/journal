#对于handler两种挂载方式的疑问
  1.按阶段挂载
    ngx_http_handler_pt     *h;
    ngx_http_core_main_conf_t   *cmcf;    
    cmcf = ngx_http_conf_get_module_main_conf(cf,ngx_http_core_module);
    //函数注册到CONTENT阶段
    h = ngx_array_push(&cmcf->phases[NGX_HTTP_CONTENT_PHASE].handlers);
    *h = ngx_http_hello_handler;  //真正处理请求的函数
    
	疑问，对于这种挂载方式,为什么当有一个空location的时候,仍然会执行
	ngx_http_hello_handler方法？
	location ~ ^/test {
    	//这里面没有任何指令,没有hello_string指令,为什么也会执行hello_handler?
		//用第二种绑定就不会执行
    }


  2.另一种挂载方式,按需挂载
    ngx_http_core_locl_conf_t clcf;
	clcf = ngx_http_conf_get_module_loc_conf(cf,ngx_http_core_module);
    clcf->handler = ngx_http_hello_handler;
 
  疑问,对于这两种挂载方式,ngx_http_core(main|loc)_conf_t这两个结构是从
  ngx_http_core_module模块拿出来的,难道locl_conf、main_conf只有一个实例吗
  那clcf->handler这种方式注册的handler岂不是整个nginx就一个?
