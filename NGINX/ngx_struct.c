 //ngxin主配置结构
 typedef struct {
    ngx_array_t                servers;         /* ngx_http_core_srv_conf_t */

    ngx_http_phase_engine_t    phase_engine;

    ngx_hash_t                 headers_in_hash;

    ngx_hash_t                 variables_hash;

    ngx_array_t                variables;       /* ngx_http_variable_t */
    ngx_uint_t                 ncaptures;

    ngx_uint_t                 server_names_hash_max_size;
    ngx_uint_t                 server_names_hash_bucket_size;

    ngx_uint_t                 variables_hash_max_size;
    ngx_uint_t                 variables_hash_bucket_size;

    ngx_hash_keys_arrays_t    *variables_keys;

    ngx_array_t               *ports;

    ngx_uint_t                 try_files;       /* unsigned  try_files:1 */

    ngx_http_phase_t           phases[NGX_HTTP_LOG_PHASE + 1]; 
 } ngx_http_core_main_conf_t;

//nginx基本数据类型core/ngx_config.h
 typedef intptr_t		ngx_int_t;
 typedef uintptr_t		ngx_uint_t;
 typedef intptr_t		ngx_flat_t;
 
 其中intptr_t、uintptr_t位置在/usr/include/stdint.h中

//nginx数组
typedef struct {
	void 			*elts;   //指向实际的数据存储区域
	ngx_uint_t		nelts;   //数组实际元素个数
	size_t			size;    //数组单个元素的大小,单位字节
	ngx_uint_t		nalloc;  //数组容量
	ngx_pool_t		*pool;  
} ngx_array_t;

//字符串数据类型core/ngx_string.h/c
 typedef struct {
	size_t		len;
	u_char		*data;	
 } ngx_str_t;

 typedef struct {
	ngx_str_t	key;
	ngx_str_t	value;
 } ngx_keyval_t;

 typedef struct {
	unsigned	len:28;  //位字段宽度是28

	unsigned	valid:1;
	unsigned	no_cacheable:1;
	unsigned	not_found:1;
	unsigned	escape:1;
	
	u_char		*data;	
 } ngx_variable_value_t;

//nginx内存池
 struct ngx_pool_s {
	 ngx_pool_data_t		d;    //这是一个链表,可以连接很多ngx_pool_t
	 size_t					max;  //分配的内存大小(对于用户来说可用的)
	 ngx_pool_t				*current; //指向d链表中的一个ngx_pool_t结构(最近一次操作的pool)
	 ngx_chain_t			*chain;
	 ngx_pool_large_t		*large; 
	 ngx_pool_cleanup_t		*cleanup; //在清理内存是回调用这个机构里的handler
   	 ngx_log_t				*log;
 };

 typedef struct ngx_pool_s	ngx_pool_t;

//nginx缓存链
 typedef struct ngx_chain_s ngx_chain_t;
 struct ngx_chain_s {
	ngx_buf_t		*buf; //缓存
	ngx_chain_t		*next;
 }

//池中数据的数据结构
 typedef struct {
	u_char		*last;
	u_char		*end;
	ngx_pool_t	*next;
	ngx_unit_t	failed;  //分配内存时失败的次数?
 } ngx_pool_data_t;

//当实际需要的内存比内存池创建是指定的size大时,则直接在该结构
//分配一段内存
 struct ngx_pool_large_s {
	ngx_pool_large_t	*next;
	void				*alloc;
 }

//释放内存时如果指定了这个结构,那么会依次调用该结构中的handler
 typedef struct ngx_pool_cleanup_s ngx_pool_cleanup_t;
 struct ngx_pool_cleanup_s {
	ngx_pool_cleanup_pt		handler;
	void					*data;
	ngx_pool_cleanup_t		*next;
 }

//nginx数组
 struct ngx_arry_s {
	void		*elts;	//指向具体数据的指针
	ngx_uint_t	nelts;  //实际包含的元素数量
	size_t		size;	//单个数组元素的大小
	ngx_uint_t	nalloc; //整个数组可容纳的个数
	ngx_pool_t	*pool;
 }

//nginx队列
 typedef struct ngx_queue_s ngx_queue_t;

 struct ngx_queue_s {
 	ngx_queue_t	 *prev;
	ngx_queue_t  *next;
 }

 --获取该队列所在的原始数据节点
 #define ngx_queue_data(queue,type,link)   \
		(type *)((u_char *)q - offsetof(type,link))


//nginx hash结构
 typedef struct {
    //看成是一个二维数组
    //第一维代表一个个的桶,每个桶又是一个数组,用来放置具体提的元素
	//相当于java中Entry[] table的Entry实体,Entry在java中代表一个桶,这个桶是一个链表
    //nginx中用数组解决hash冲突,java中用链表
	ngx_hash_elt_t	**buckets; 
	ngx_uint_t		size;
 }

//hash 元素
 typedef struct {
	void 		*value;   //存入的值
	u_short		len;      //值得长度
	u_char		name[1];  //key值  为什么数组长度是1 ??
 } ngx_hash_elt_t;


//用于Nginx在解析配置文件时描述每个指令的属性
 struct ngx_conf_s {
	char				*name; //指令名字 ?
	ngx_array_t			*args; //指令后的参数的值 ?
	ngx_cycle_t			*cycle;
	ngx_pool_t			*pool; 
	ngx_pool_t			*temp_pool;	//解析配置文件时临时内存池,解析完后就释放
    ngx_conf_file_t 	*conf_file;	//存放nginx配置文件的相关信息,比如文件名nginx.conf
    ngx_log_t			*log;	//日志文件的相关属性
	void				*ctx;	//描述指令的上下文
	ngx_uint_t			module_type; //该指令支持的模块core、http、event和mail的一种
	ngx_uint_t			cmd_type;	//指令的类型?
	ngx_conf_handler_pt	handler;	//指令自定义的处理函数
	char				*handler_conf; //自定义处理函数的相关配置
 }

//表示文件的结构
 typedef struct {
	ngx_file_t		file;  //文件信息,描述符fd、路径等
	ngx_buf_t;		*buffer; //文件的所有数据
	ngx_uint_t		line; //实际读的行数?
 } ngx_conf_file_t;


//表示nginx命令的结构
 struct ngx_command_s {
	ngx_str_t			name; //命令名字
	ngx_uint_t			type; //命令可以用在那些地方
					 //解析该命令的函数,该函数返回char *
	char				*(*set)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
	ngx_uint_t			conf;  //用哪个区域的pool为该命令分配内存
	ngx_uint_t			offset; //该命令在自定义结构体中的偏移位置
	void				*post;	// ?
	

 }



