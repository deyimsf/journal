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

#nginx基本数据类型core/ngx_config.h
 typedef intptr_t		ngx_int_t;
 typedef uintptr_t		ngx_uint_t;
 typedef intptr_t		ngx_flat_t;
 
 其中intptr_t、uintptr_t位置在/usr/include/stdint.h中

#字符串数据类型core/ngx_string.h/c
 
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






