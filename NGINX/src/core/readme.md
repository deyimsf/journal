_s结尾代表结构体,例如
	struct ngx_pool_cleanup_s{
	
	};


_t结尾代表重新定义的一个类型,例如
	typedef struct ngx_pool_cleanup_s ngx_pool_cleanup_t;


_pt代表一个函数类型,可以用来定义一个函数,例如
	typedef void (*ngx_pool_cleanup_pt)(void *data);

	//定义一个该类型的变量
 	ngx_pool_cleanup_pt 	handler;	
	
	//定义一个int类型变量
	int	var_int;

	//去掉typdef就表示一个普通的函数指针,例如
	void (*ngx_pool_cleanup_pt)(void *data);
	此时ngx_pool_cleanup_pt就只代表一个变量(函数变量)













