#nginx拦截错误代码,默认是off
proxy_intercept_errors on;

##安装Nginx 
1.GCC---GNU编译器集合 
  gcc是一个开源编译器集合，是用于处理各种各样的语言：C、C++、java 
  、Ada、FORTRAN，等等。 
  $yum install gcc 
2.PCRE库 
   在Nginx编译需要PCRE，因为Nginx的Rewrite模块和HTTP核心模块会使用 
   到PCRE正则表达式。需要安装两个安装包pcre和pcre-devel。第一个安装 
   包提供编译版本的库，而第二个提供开发阶段的头文件和编译项目的源代码。 
  $yum install pcre pcre-devel 
3.zlib库 
   zlib库提供了开发人员的压缩算法，在Nginx的各种模块中需要使用gzip压缩。 
  $yum install zlib zlib-devel 
4.OpenSSL 
   在Nginx中，如果服务器提供安全网页时则会用到OpenSSL库，我们需要安装库文件 
   和它的开发安装包。 
  $yum install openssl openssl-devel 
5.下载并安装Nginx 
  $ wget http://nginx.org/download/nginx-0.7.66.tar.gz 
  $ tar zxf nginx-0.7.66.tar.gz 
  $ cd ./nginx-0.7.66 
  $ ./configure 
  $ make 
  $ make install 


##启动nginx 
$ /usr/local/nginx/sbin/nginx 

##立即停止守护进程(使用SIGTERM信号) 
$ /usr/local/nginx/sbin/nginx -s stop 

##温和地停止守护进程(使用SIGQUIT信号) 
$ /usr/local/nginx/sbin/nginx -s quit 

##重新打开日志(使用SIGUSR1信号)
$ /usr/local/nginx/sbin/nginx -s reopen 

##重新载入配置文件(使用SIGHUP信号)
 $ /usr/local/nginx/sbin/nginx -s reload 
 $ kill -SIGHUP master_pid
 
 master接收到该信号后会,先重新加载配置文件,然后重启新的worker并
 接收新的请求,老的worker收到master的停止接收新情求的信号后,处理
 完当前的请求后退出。
 
##配置文件默认位置 
/usr/local/nginx/conf/nginx.conf 

##指令总是以分号结尾 
worker_processes 1; 

##location 区段 
Nginx允许你定义location区段，通过指定的模式与客户端请求的URI相匹配。 
   语法: location [=|~|~* |^~ |@] /uri/{...} 
   修饰符： 
  1)[=]: URI的定位必须与指定的模式精确匹配。该模式在这里限定为一个简单的字符串， 
  不能 使用正则表达式。 
        server{ 
        server_name website.com; 
        location = /abcd { 
          #对该位置的访问： 
          a>可用http://website.com/abcd 
          b>可用http://website.com/ABCD (如果操作系统区分大小写就不可访问) 
          c>可用http://website.com/abcd?param1&param2 
          d>不可用http://website.com/abcd/ (结尾斜杠) 
          e>不可用http://website.com/abcde 
        } 
        } 
  2)[无]: URI的定位必须以指定模式开始，不可以使用正则表达式。 
         server{ 
         server_name website.com; 
         location /abcd{ 
           #对该配置的访问 
             a>可用http://website.com/abcd 
             b>可用http://website.com/ABCD   (大小写看操作系统) 
             c>可用http://website.com/abcd?param1&param2 
             d>可用http://website.com/abcd/ 
             e>可用http://website.com/abcde (只要以/abcd开头就可以) 
         } 
         } 
  3)[~]: 客户端请求的URI与指定 的正则表达式匹配必须区分大小写。 
        server{ 
        server_name website.com; 
        location ~ ^/abcd${ 
          #对该配置的访问 
            a>可用http://website.com/abcd 
            b>不可用http://website.com/ABCD  (区分大小写) 
            c>可用http://website.com/abcd?param1&param2 (不过滤查询字符串) 
            d>不可用http://website.com/abcd/ 因指定了正则表达式 
            e>不可用http://website.com/abcde 因指定了正则表达式 
        } 
        } 
  4)[~*]: 对客户端请求的URI与指定的正则表达 式匹配， 不区分大小写。 
        server{ 
        server_name website.com; 
        location  ~* ^/abcd${ 
          #对该配置的访问 
            a>可用http://website.com/abcd 
            b>可用http：//website.com/ABCD 
            c>可用http://webstie.com/abcd?param1&param2 
            d>不可用http://website.com/abcd/ 
            e>不可用http：//website.com/abcde 
        } 
        } 
  5)[^~]: 类似于无标志行为(2),URI的定位必须以指定模式开始。不同的是，如果模式匹配，那么Nginx就停止搜索其他模式 
  6)[@]: 定义命名location区段，这些区段客户端不能访问，只可以由内部产生的请求来访问，例如try_files 或 error_page                          

##Nginx在下面的顺序中搜索匹配模式 ？？？？？ 
  1.带有=修饰符的location区段， 如果指定字符串与请求的URI精确匹配，则Nginx使用该location的设置。 
  2.没有修饰符的location区段，如果指定字符串与请求的URI精确匹配，则Nginx使用该location设置。 
  3.带有^~修饰符的location， 如果指定字符串与请求的URI开始匹配，则Nginx使用该location设置。 
  4.带有~或~*修饰符的location区段， 如果正则表达式与请求的URI匹配，则Nginx使用该location设置。 
  5.没有修饰符的location区段， 如果指定字符串与URI请求开始匹配，则Nginx使用该location的设置。 
  
    例子1：请求http://website.com/document 
    server{ 
    	server_name website.com; 
    	location /doc{ 
    		... 
    	} 

    	location ~* ^/document${ 
    		#会匹配该配置，因为可以精确匹配"document" 
    	} 
    } 
   例子2:请求http://website.com/document 
   server{ 
	    server_name website.com; 
	    location /document{ 
   	   	   #会匹配该配置，因为是精确匹配并且没有修饰符的location区段(精确匹配)优先级比正则(精确匹配)高 
   	    } 
   		location ~* ^/document${ 
   			... 
   		} 
   } 
   例子3:请求http://websit.com/document 
   server{ 
   		server_name website.com; 
   		location ^~ /doc{ 
   			#匹配该配置，^~的优先级比~或~*高 
   		} 
   		location ~* ^/document${ 
   			... 
   		} 
   } 
  

##windows下Nginx的操作 
   start nginx    启动nginx 
   Nginx -s stop         快速关闭Nginx，可能不保存相关信息，并迅速终止web服务。 
   Nginx -s quit         平稳关闭Nginx，保存相关信息，有安排的结束web服务。 
   Nginx -s reload       因改变了Nginx相关配置，需要重新加载配置而重载。 
   Nginx -s reopen       重新打开日志文件。 

##正则表达式的11种元字符 
  ^    行开始 
  $    行结束 
  .    任何字符 
  []   匹配指定集合内的任意字符 
  [^]  匹配不包括在指定字符集内的字符串 
  |    或 
  ()   分组 
  \    转义 
  *    0或多次 
  +    1或多次 
  ?    0或1次 
  {x}  x次 
  {x,} 至少x次 
  {x,y}x到y次 
可以用$1,$2...等来捕获子表达式 
  
##Nginx代理模块 
[proxy_pass]:指定转发到后端服务器的请求，在location中指示 
  对于TCP套接字，语法如下： 
     proxy_pass http://hostname:port; 
     proxy_pass http://unix:/path/to/file.socke; ???? 
  也可以指向upstream区段 
  例子： 
  proxy_pass http://localhost:8080; 
  proxy_pass http://127.0.0.1:8080; 
  proxy_pass http://unix:/tmp/nginx.sock; 
  proxy_pass http://192.68.0.1; 
  proxy_pass http://localhost:8080/uri/; 
  proxy_pass http://$server_name:8080; 
  使用upsteam区段 可以实现负载 
  upstream backend{ 
  server 127.0.0.1:8080; 
  server 127.0.0.1:8081; 
  } 
  location ~* \.php${ 
  proxy_pass http://backend; 
  } 
  
  location xxx{ 
  rewrite a b; 将a替换为b，例如：rewrite ^/search/(.*)$ /search.php?q=$1; 
  } 

#nginx变量
 nginx的变量在配置被加载的时候创建;在请求实际处理的时候才会赋值。
 所以无法在请求时动态的创建变量,并且使用未被定义的变量会无法启动。
 例子：
	location  /var {
		echo 	"var = $var";
	}
 由于$var未定义所以启动失败.
 
 一旦变量被创建，则整个nginx配置都可见，但并不意味着该变量值被共享，
 每个请求都有自己对这个变量赋值的副本，相互不受影响.
 例子：
	location /var {
		echo	"var = $var";
	}
    location /var2 {
		set $var "var";
		echo "var = ${var}2";
	}
 如果对其访问，则
	/var输出    var = 
	/var2输出	var = var
 
 nginx变量的生命周期存在于一个请求内，包括内部跳转,与其它无关
	location /var {
		set $var "var";
		echo_exec	/var2;
		#rewrite ^	/var2;
	} 
	location /var2 {
		echo	"var = -- $var --"
	}
 输出结果:
	var = -- var --

 以上是自定义变量，还有一些各个模块的内置变量，比如核心模块
 的$arg_xx、$args、$sent_http_xx等.
 大部分内置变量是不能修改的,有些可以,如$args,$arg_xx,该变量存放
 请求的querystring(未解码),如:
	location /args {
		echo "args=[$args]";
	}
 用url /args?a=b&c=11中11 访问则会输出:
	args=[a=b$c=11%E4%B8%AD11]
 
 像arg_xx内部变量会args从解析出对应的xx变量值,比如
	location /args {
		set		$orig_args	$args;	
		set 	$args "a=1&b=2";
		echo	"orig_args=[$orig_args]";
		echo 	"args=[$args]";
	}
 用url /args?a=3&b=4 访问该location,则输出:
	orig_args=[a=3&b=4]
	args=[a=1&b=2]

 标准ngx_map模块的map配置指令
	map $args $foo {
		default		a;
		debug		b;
	} 	
    server {
		listen 80;
		
		location /test {
			echo	"args=[$args]";
			echo	"foo=[$foo]";
		}
	}
 以上表示将$args对$foo的一个映射，但并不是将$args的值给$foo
 而是说当$args这个变量的值是debug时,$foo的值是b,否则是a.
  	 $curl	'http://localhost/test?debug'
	  --> args=[debug]
	  --> foo=[b]
 map指令在只能在server外部，执行过程大概如下描述：
  当请求过来后，ngxin去调用函数去取args参数的值，在取得过程中
  会map指令做考虑(猜测,为看源码). 

 子请求,就是不是通过客户端请求来的不走HTTP协议的请求,但也不是内部跳转,
 内部跳转使用的变量和整个请求共享,但是子请求一般不共享变量。也有共享的如
 ngx_auth_request模块,一般都会禁用其父子共享变量。
 #--------------------------------------------------------------------
 注意:所谓的内部跳转就是强行将当前阶段退回到find-config阶段,这样就可以
      让URI重新和location块进行匹配.
 #--------------------------------------------------------------------
 例子:
	location /main {
		echo			"main[$args]";
		echo_location /sub1;
		echo_location /sub2 "a=1&b=2";
	}

	location /sub1 {
		echo  "sub1[$args]";
	}
	location /sub2 {
		echo   "sub2[$args]";
	}
 访问/main?c=3,则会串行执行并输出,不管对错。
   	main[c=3]
	sub1[]
    sub2[a=1&b=3]
 同时我们也看到各个请求之前的变量相互独立。

 但不是所有的内置变量都作用于当前请求。指作用于第一次的请求,以后的
 子请求只用之前的值,比如核心模块的$request_method变量。
 例子：
	location /main {
		echo "main method=[$request_method]";
		echo_location /sub1;
	}
	location /sub1 {
		echo "sub method=[$request_method]";
	}
 用POST方式访问输出结果:
	main method=[POST]
	sub method=[POST]
 用GET方式访问输出结果:
	main method=[GET]
	sub  method=[GET]
 因为echo_location 是按GET方式请求的,所以根据结果可得知，
 $request_method变量无法获取子请求的method。为了获取子请求的method
 我们可以使用第三方ngx_echo模块提供的$echo_request_method变量。
 
 注意:以上说的所谓子请求并不是真正的发出HTTP请求,只是在nginx内部调用
  几个函数,并没有走socket之类的网络。

 Nginx中的变量有三种值:字符串、无效值、not found。
 比如$var这个变量只创建但并未赋值时,则$var就是无效的(invalid);如果请求
 中根本没有arg_xxx这个参数，则就是not found.

#Nginx指令的执行顺序
 nginx在处理用户请求的时候都是按不同阶段一次处理的。
 nginx模块提供的配置指令一般都会注册在某个处理阶段,但并非所有
 的指令都会绑定某个阶段，比如geo指令和map指令属于声明性的，不
 直接产生某种动作。
 nginx请求处理阶段有11个，执行的时候按阶段先后顺序依次处理在该
 阶段绑定的指令。11个阶段依次如下：
	post-read 当nginx读取并解析完请求头后开始运行
 	server-rewrite
	find-config 该阶段不支持模块注册,而是由ngxin核心来完成请求与location
                配置的匹配工作,也就是说再次之前没有任何请求与location配置
 	            块相关联。所以对于运行在该阶段之前的阶段,他们的指令应该配
	            置在location块之外,否则不起作用。
	rewrite
	post-rewrite 不接受注册,如果在rewrite阶段指定了要做内部跳转,那么该阶段
		    	 就会将当前请求强制退回到find-config阶段。
	preaccess
	access 		 运行在该阶段的指令多是执行访问控制性质的任务,比如检查
		   		 用户的访问权限,检查用户的来源Ip地址是否合法等。
	post-access  不接受注册,主要用于配合access阶段实现核心模块的satisfy功能
	try-files	 不接受注册,专门用于实现try_files指令的功能
	content
	log

 看下面的例子：
	location /test {
		set  $a 1;
		echo $a;
	
		set  $a 2;
		echo $a;
	}
 访问 /test 输出
 	2
    2
 因为set指令绑定在rewrite阶段,echo指令绑定在content阶段
 所以会先执行完set指令后再指令echo指令。













#----------------ngx_http_geo_module------------------
 该模块用客户端的ip地址创建变量
 	geo $geo {
		default		    "a";
	
		127.0.0.1	    "b"; 
		127.0.0.1/16    "c";
		127.0.0.1/32    "d";		
 	}
 如果客户端ip地址是127.0.0.1则优先匹配最长的子网掩码,
 没写子网掩码则默认是32,并且优先于有子网掩码的。实际上
 以上的配置除啦127.0.0.1其他都没什么意义。
 所以上面例子$geo变量值是b,如果没有这个配置,则变量值为d。

 默认情况nginx从$remote_addr获取IP地址；可以让其从其他变量
 获取ip地址，比如：
	geo $arg_ip  $geo {
		default			"a";
		192.168.195.1   "b";
    }



#----------------HttpRedis2Module----------
#指令
 #redis2_connect_timeout
  和redis建立链接的超时时间,默认单位秒.
  支持单位:s(秒)、ms(毫秒)、y(年)、M(月)、w(周)、d(天)、h m 

 #redis2_send_timeout
  发送请求命令到redis的超时时间. ?非阻塞模式下如何计算的,貌似通过系统参数设置
  如：TCP_USER_TIMEOUT

 #redis2_read_timeout
  从redis接收响应的超时时间.

 #redis2_buffer_size
  从redis读取响应的缓存大小

 #redis2_next_upstream
  指定失败条件,这个条件成立后，请求转发到另一个server.
  前提是redis2_pass 后面的upstream有多个server。

#链接池
 例子：
	upstream backend{
		server 127.0.0.1:6379
		keepalive 1024; //池大小
	}
	
    server{
		location = /redis{
			redis2_pass backend;	
		}
	}

#nginx中引用文件指令的当前路径
 在安装Nginx的时候有一个prefix参数,比如我本机安装时用的是:
 	--prefix=/My/work/nginx/nginx
 
 include的指令的当前路径在conf目录下,范围在server、http、location等
 区域,没什么限制.
    location /path {
        include   domains/cc.conf;  //domains目录必须在--prefix/conf/下。
    }

 content_by_lua_file指令的当前目录和conf同目录,范围location、location if
	location /path {
		content_by_lua_file	path.lua; //path.lua一定和--prefix 同目录。
	}

#指令set和set_by_lua混合使用的例子
  location /test {
		set $a	10;
		set $b	20;
		set_by_lua $c "return ngx.var.a + ngx.var.b";
		set $content "$a + $b = $c";
	
		echo $content;	
  }
 访问 /test 输出结果
	10 + 20 = 30
 从上面可以看到,属于不同模块的指令,有序的执行了。因为nginx中只有
 字符串,所以用到了lua对加数自动类型转换的特性,这样就计算出了两个
 数字的和,然后再利用nginx的”变量插值“将字符串输出。
 
 第三方模块ngx_lua的set_xx指令之所以能和ngx_rewrite模块的set指令混合使用
 ,并且指令执行顺序和配置指令的顺序相同,是不仅因为这两个指令都在rewrite阶
 段,因为它借助了第三方模块ngx_devel_kit,将自己的配置指令融合到了ngx_rewrite
 模块中。也就是说,如果第三方模块A和B的指令也在rewrite阶段,但是并没有使用
 ngx_devel_kit模块进行融合,那么不管他们的配置指令是否交叉,指令执行的时候
 一定是先执行完一个模块的指令后在执行另一个模块的指令。(具体哪个模块的指令
 向执行,一般有模块的加载顺序决定,有例外)。

#more_set_input_headers指令(属第三方ngx_headers_more模块)
 该指令总是运行在rewrite阶段的末尾,用于操纵当前请求的请求头,还可以
 在请求头不存在时自动创建。
 例子:
	location /text {
		set $value	 aaa;
		more_set_input_headers   "Head-Masf:$value";
		set $value	 bbb;
	
		echo "Head-Masf:$http_head_masf";
	}
 这个例子是在修改或创建请求头Head-Masf的值.另外$http_xxx变量在匹配请求头时
 会自动将请求头的字母转换为小写字母,同时把"-"转换为"_",所以匹配请求头Head-Masf
 需要用$http_head_masf变量.最后因为more_set_input_headers指令总在rewrite末尾
 执行,所以输出结果为 Head-Masf:bbb
 
 这个例子证明了即使运行在同一个请求处理阶段,所属不同模块的指令也可能会分开
 独立运行.
 ps:rewrite_by_lua指令也运行在rewrite的末尾阶段.

 //清理响应头
 more_clear_headers 'Content-Type';
 //设置响应头
 more_set_headers   'Content-Type: text/plain'

#标准模块ngx_access的allow和deny指令用于控制哪些Ip地址可以访问,哪些不可以
 这两个指令运行在access阶段.
 例子:
	location /test {
		allow	127.0.0.1;
		deny	all;

		echo "hello";
	}
 访问 /test 如果远端ip地址不是127.0.0.1则返回403状态.

#单个阶段同时运行多个模块的指令问题
 一般情况下,单个阶段绑定的不同模块的指令可以同时使用,但是content
 阶段则例外.大多数模块在向content阶段注册指令时,都是在当前的location
 配置块中注册content handler(内容处理程序). 值得注意的是每个location
 只能有一个content handler,因此当同时有多个模块向content阶段注册指令
 时,只会成功一个,具体哪个成功则不确定。但是同一个模块的不同指令没限制。
 例如:
	location /test {
		echo 	hello;
		content_by_lua 'ngx.say("world hello")';
		echo	world;
	}
 访问 /test 可能输出
      hello
      world
 这个例子中,echo和content_by_lua指令同属content阶段但不属同一个模块,
 因此只有一个指令可以注册成功.

 如果想看到我们预期的输出,可以使用echo_before_body等指令,例如:
	location /test {
		echo_before_body	hello;
		content_by_lua	'ngx.say("world hello")';
		echo_after_body		world;
	} 
 访问 /test 则输出
	 hello
	 world hello
	 world
 之所输出这样的结果,因为Nginx在输出响应体数据时调用"输出过滤器",这两
 条指令就在数据输出之前和之后调用。需要注意的是"输出过滤器"并不属于
 Nginx定义的11个阶段。 
	
#content阶段的静态资源服务模块
 Nginx会在该阶段安排三个静态资源服务块,当在content阶段没有注册任何指令
 的时候,也就是说在 location / {} 块中没有配置任何content阶段的指令时会
 相应的触发ngx_index、ngx_autoindex、ngx_static模块.

 ngx_index和ngx_autoindex 只会作用于URI以 / 结尾的请求,例如GET /cc/,
 如果不是以 / 结尾则把控制权交给下一个模块。而ngx_static模块值处理不以 / 结尾的请求。
 ngx_index模块主要用于在文件系统目录中自动查找指定的首页文件。
 例如:
	location /bb/ {
		root /var/www/;    //用root来指定"首页"所在的目录
		index index.htm  index.html shouye.ht; //用index指定"首页"文件名
	    #autoindex on;   //自动生成目录索引,默认off
	}
 访问 /bb/ 因为以 / 结尾所以ngx_index模块会起作用,执行顺序如下:
	在/var/www/bb/ 目录下按顺序查找index指令指定的文件名,假设第一个文件index.htm
    存在,则直接发起"内部跳转"到 /bb/index.htm 这个新的uri,然后重新匹配location。
     
    如果我们没有 location /bb/index.htm {} 这样的location,那么就会重新匹配到我们的
    location /bb/ {},这次因为不是以 / 结尾的,所以ngx_index模块不再起作用。然后交给
    content阶段的下一个模块,如果ngx_autoindex模块的autoindex 指令是on则就会自动生成
    一个当前目录的索引,然后就可以在浏览器上看到当前目录都有哪些内容。

    因为我们这里没有设置autoindex指令,所以这里下一个模块是ngx_static。 
     
    ngx_static模块会根据root指令的配置确认.../bb/index.htm文件是否存在,如果存在则
    直接输出,并附加上一些响应头信息。如果不存在则抛出404。

    ngx_static模块是content阶段默认垫底的最后一个模块,该模块才是真正把静态文件展示
    出去的功臣。

    如果遍历到最后都没有在指定的目录下找到index 配置的文件名,则交给content阶段的下一
    个模块。

 注意:在上面我们提到了,当找到指定文件后并不是直接输出,而是就发起了一个内部跳转,但是
 如果这个时候我们有这样一个locaiton：
    location /bb/index.htm {
	    echo "ccccccc";
	}		
 那么最终输出给客户端的数据是cccccc,因为location在匹配的时候用的是"贪婪"匹配,所以会先
 匹配/bb/index.htm,又因为存在content阶段的指令echo,所以不会触发静态模块结果就被截胡了。

#ngx_static模块(content阶段的垫底模块)
 看这样一个例子:
	location / {
		root  /My/work/nginx/nginx/html/ ;
	} 
 在/html/目录下有hello.html index.html两个文件,且内容同文件名.
 我们访问 /hello.html 则输出
	hello
 他的执行顺序是这样的:
	首先,因为我们的location没有配置任何运行在contenet阶段的指令,所以location的
	content handler(内容处理程序)就没有被注册,这时候处理权落到了content阶段的三
	个静态资源服务模块中。
	
	首先运行的ngx_index和ngx_autoindex模块并没有看到以 / 结尾的URI,所以直接放弃,
	接着处理权落到了ngx_static模块。该模块根据root指令的配置,确认.../html/hello.html
    文件是否存在,如果存在则将他们的内容输出,并自动设置Content-Type、Content-Length
    以及Last-Modified等响应头。
 如果上面的location下没有配置root指令,那么默认用--prefix的目录

 另外一个需要注意的是如果你的location并不是在提供静态服务,一定注意在
 location 下是不是这是了 content 阶段的指令,如果没有的话就会走静态模块。
 例如：
	location /get {
		set	$a = b;
	}
 如果访问 /get 则该请求最终会交给ngx_static模块处理,该模块会去文件系统
 上找 [--prefix目录]/get 这个文件,如果你的目录下没有get这个文件,那么就
 会抛出404错误。

#set_real_ip_from指令,ngx_realip标准模块默认未开启,--with-http_realip_module
 这个指令在第一个阶段post-read执行,该指令也注册到了preaccess阶段
 例子: 	
	set_real_ip_from 127.0.0.1;
	real_ip_header	Real-Ip;
 	location /real_ip {
		echo "from:[$remote_addr]";
	}
 本机访问/real_ip 并指定请求头Real-Ip
   $curl -H 'Real-Ip:192.18.1.1' /real_ip
 输出
	from:[192.18.1.1]
 
#rewrite指令,属ngx_rewrite模块
 语法: rewrite regex replacement [flag];

 该指令只是在做简单的URI重写,不会立即进行内部跳转,而是简单的指定有必要
 在随后的post-rewrite阶段发生内部跳转。
 例子:
 	location /foo {
		rewrite ^ /bar;
		rewrite ^ /baz;

		echo 	"aaa";
	}
	location /bar {
		echo 	"bbb";
	}
	location /baz {
		echo 	"ccc";
	}
 访问 /foo 执行完两个rewrite指令后,URI最终被修改为/baz,之后进入post-rewrite
 阶段,该阶段执行内部跳转将请求强行退回到find-config阶段,然后最终匹配到/baz
 这个location,输出结果
	ccc
 从执行顺序可以看到/foo中的 echo 指令因为在content阶段,所以根本没有机会执行

#satisfy指令,属于核心标准模块
 该指令主要用于对注册在 access 阶段的各个模块的程序进行协调。
 例子:
	location /test {
		satisfy 	all;
	
		deny 		all; //该指令注册在access阶段	
		access_by_lua 'ngx.exit(ngx.OK)'; //注册在access阶段

		echo 	"hello";
	} 
 上面的例子表示,只用在access阶段的指令都同意先下走时,才会输出 hello 字符.
 对于上面,因为deny 指令总是拒绝先下走,所以会直接返回403。
 如果将例子中的 satisfy 赋值 any 则表示只要有一个access阶段的指令同意就可以
 输出 hello字符。注意:默认satisfy的值是all。

#try_files指令,属标准核心模块
 该指令接收两个以上的参数,每个参数都是一个URI,如果指定的URI后面带有"/"则
 会直接抹掉这个斜杠。
 例子:
	root /var/www/;
	location /test {
		try_files /foo /bar/ /barz;
		echo "uri:$uri";
	}
 执行顺序是这样的,该指令会依次把前n-1个参数映射为文件系统上的对象,然后检查
 是否存在.如果遇见存在的就不在向后检查,会把当前URI改写为对应的URI,比如访问
 /test 时如果/var/www/foo这个文件存在,那么URI就会被改写为/foo,然后走向下一
 个阶段。如果直到检查完第n-1个参数都没有发现对应的文件,该指令就会立即跳转到
 第N个参数所指定的URI,对于上面的例子会内部跳转到/barz这个URI。


#nginx做本地缓存(模拟cdn?)
 配置文件如下:
 	http{
		#模拟后端应用,比如tomcat
       	server {
			listen 8080;
		
			location ~* ^/app.action {
                #设置前端可以缓存的时间,对于http/1.1协议,下面两个头都可以,但
 				#max-age的优先级高; 对于1.0则只识别Expires头.
				Expires: Fri, 13 Feb 2016 13:30:27 GMT;  
				Cache-Control: max-age=60;
				
				#设置Last-Modified头,决定客户端是否返回304
 				Last-Modified: Fri, 13 Feb 2015 13:30:27 GMT	
				
 				#输出内容
				content_by_lua 'ngx.say(os.clock())';
			}
		}        

        
		#--------------------华丽的分割线-----------------------
		#后端应用
		upstream tomcat_ {
			server 127.0.0.1:8080 weight=1;
		}
	
	    #分别代表:缓存的文件存入磁盘的位置; 存入文件深度;  共享内存的名字和大小
		proxy_cache_path /My/work/nginx/nginx/ccc levels=1:2 keys_zone=abc:1m;	

	    #前端代理nginx
		server {
			listen 80;
			server_name test;
		
			location ~* ^/cdn {
				rewrite /  /app.action break;  #rewrite到后端应用

				#指定共享缓存名字
				proxy_cache abc;       
				#指定缓存key,如果当前参数name=zs, 那么
                #这个缓存key就是 test/app.actionzs
				proxy_cache_key  $host$uri$arg_name 
				#指定对那种响应状态码缓存,以及缓存时间,
				#这里的时间可以被后端应用的Expires和Cache-Control:max-age=60覆盖 				
				proxy_cache_valid 200 301 302  1s;
				#使用HTTP/1.1协议请求后端应用
				proxy_http_version 1.1;

				proxy_pass http://tomcat_;
			}
		} 
	}  

   注意:使用proxy_cache指令,缓存虽然过期了,但是并没有被删除,可以用purge模块的 
     proxy_cache_purge 指令去删除缓存。
	 例如:
		location ~ /purge {
			#abc是共享缓存的名字
			proxy_cache_purge abc $arg_key;
		}	

#Nginx代码的目录结构
  core: 存放core module的代码,也是ngxin服务的入口
  http:	http core module的代码,ngxnx作为web/http proxy server运行时的核心模块
  mail: mail core module代码,nginx作为pop3/imap/smtp proxy server的核心模块
  event:ngxin自身对事件处理逻辑的封装
  os: nginx对各个平台抽象逻辑的封装
  misc: nginx的一些utils,定义了test和profiler的一些外围模块的逻辑

#NGINX处理请求大致流程
  1.客户请求开始

  2.从ngx_http_init_request开始处理请求,该函数会设置请求事件为
    ngx_http_process_request_line函数

  3.ngx_http_process_request_line用来处理请求行,在该函数中会调用
	ngx_http_read_request_header函数来读取请求。而处理请求用
	ngx_http_parse_request_line函数来处理请求行。
    当请求行处理完毕后,Nginx就会改变Tcp读事件的Handler为
    ngx_http_process_request_headers函数。

  4.到此,在遇到读事件后就会调用ngx_http_process_request_headers函数
    该函数的作用就是循环读取解析一系列的请求头。过程如下:
	调用ngx_http_read_request_header函数读请求头数据
	调用ngx_http_parse_header_line函数处理请求头
	将解析到的名字在ngx_http_headers_in中找对应的handler并调用
  
  5.请求头处理完成后调用ngx_http_process_request来处理请求。
    该函数将当前的读写事件处理函数都设置为ngx_http_request_handler
	而ngx_http_request_handler会根据事件的类型来调用read_event_handler
	或者write_event_handler。
    此时的read_event_handler为ngx_http_block_reading函数,即不读数据,
	也就是说此时还没有读取请求体。
	
  6.真正处理发生在ngx_http_handler函数中。?在这里处理请求体?
    该函数设置写事件函数write_event_handler为ngx_http_core_run_phases,
	并执行该函数。
	ngx_http_core_run_phases就是nginx留给我们的对外接口,总共有11个阶段。

  7.当所有的阶段处理完后会
	7.1调用header filter中的所有filter,最后一个是ngx_http_header_filter,该filter
       会调用ngx_http_write_filter将头输出。
  
	7.2调用body filter中的所有filter,最后一个是ngx_http_write_filter,该filter
	   对body进行输出。

#Ngxin中的基本数据结构	
##Nginx中的ngx_str_t字符串结构 
  src/core/ngx_string.h|ngx_string.c
  
  typedef struct {
 	size_t	len;
 	u_char	*data;
  }
 
##ngx_pool_t数据结构,用来管理资源
  src/core/ngx_palloc.h|ngx_palloc.c
 
  typedef struct ngx_pool_s	ngx_pool_t;
  struct ngx_pool_s {
 	ngx_pool_data_t		d;
 	size_t				max;
 	ngx_pool_t			*current;
 	ngx_chain_t			*chain;
 	ngx_pool_large_t	*large;
 	ngx_pool_cleanup_t	*cleanup;
 	ngx_log_t			*log;
  };
 
##nginx中的数组
  src/core/ngx_array.h|ngx_array.c
 
  typedef struct ngx_array_s		ngx_array_t;
  struct ngx_array_s {
 	void		*elts;
 	ngx_uint_t	nelts;
 	size_t		size;
 	ngx_unit_t	nalloc;
 	ngx_pool_t	*pool;
  }
 
##ngx_hash_t
  src/core/ngx_hash.h|ngx_hash.c
  
  //hash元素key-value键值对
  typedef struct {
     void             *value;   //value值
     u_short           len;	   //值的长度
     u_char            name[1]; //key   ?中括号什么意思
  } ngx_hash_elt_t;
 
  //hash结构
  typedef struct {
     ngx_hash_elt_t  **buckets;   //hash桶(size个)
     ngx_uint_t        size;		 //hash桶个数
  } ngx_hash_t;
 
  hash表中对于常用的解决冲突的方法有线性探测,二次探测和开链法等。ngx_hash_t
  中使用的是和java中HahsMap一样的方式开链法。当ngx_hash_t中的开链并不是真的
  开了一个链表。
 
  从ngx_hash_t这个hash结构可以看到,该结构有size个(*buckets),一个(*buckets)
  代表一个桶,这一个桶里面放置了好多的ngx_hash_elt_t元素,实际上每个桶指向的
  就是一个ngx_hash_elt_t元素的数组。
 
  如果和java中的HashMap对比的话,可以看到**buckets对应HashMap中的数组,而size
  就代表数组的大小。nginx中的每一个桶(*buckets)对应java中数组的一个元素。
  最后两者不同的是,java每个数组的元素用链的方式将冲突的key连接起来,而nginx中
  每个桶(*buckets)用"数组"的方式将其链接起来。
 
 
  nginx中的hash表只能一次初始化构建起整个hash表,后续不能删除和插入。
 
##ngx_hash_wildcard_t
  该hash表是为了处理带有通配符的域名匹配问题。该表支持两种通配符的域名。
  1.形如 *.aaa.com,.aaa.com,这样的key可以匹配www.aaa.com,b.a.aaa.com。
  2.形如 aaa.*,该key可以匹配aaa.com,aaa.cn等域名。
 
  需要注意的是,一个该结构类型的hash表只能匹配一种上述一种类型的key。
  ngx_hash_find_wc_head用来匹配通配符在前的key
  ngx_hash_find_wc_tail用来查找通配符在后的key
 
##ngx_hash_combined_t hash表的一个组合
  typedef struct {
 	ngx_hash_t			hash;    //普通hash表
 	ngx_hash_wildcard_t *wc_head;//通配符在前的hash表
 	ngx_hash_wildcart_t *wc_tail;//通配符在后的hash表
  } ngx_hash_combined_t;
 
  查找时会依次从这三个hash表中查找,一单找到,立即返回,只返回一个。
 
##ngx_hash_keys_arrays_t 
  src/core/ngx_hash.h|c
  该结构用来方便我们创建上面三种hash表而存在的辅助类型
 
  typedef struct {
       ngx_uint_t        hsize;
   
       ngx_pool_t       *pool;
       ngx_pool_t       *temp_pool;
   
       ngx_array_t       keys;
       ngx_array_t      *keys_hash;
   
       ngx_array_t       dns_wc_head;
       ngx_array_t      *dns_wc_head_hash;
   
       ngx_array_t       dns_wc_tail;
       ngx_array_t      *dns_wc_tail_hash;
  } ngx_hash_keys_arrays_t;

##ngx_chain_t 
  src/core/ngx_buf.h|c
  typedef struct ngx_chain_s ngx_chain_t;

  struct ngx_chain_s {
	ngx_buf_t 	*buf;
	ngx_chain_t	*next;
  }

##ngx_buf
  src/core/ngx_buf.h|c
  struct ngx_buf_s {
      u_char          *pos;
      u_char          *last;
      off_t            file_pos;
      off_t            file_last;
  
      u_char          *start;         /* start of buffer */
      u_char          *end;           /* end of buffer */
      ngx_buf_tag_t    tag;
      ngx_file_t      *file;
      ngx_buf_t       *shadow;
  
  
      /* the buf's content could be changed */
      unsigned         temporary:1;
  
      /*
       * the buf's content is in a memory cache or in a read only memory
       * and must not be changed
       */
      unsigned         memory:1;
  
      /* the buf's content is mmap()ed and must not be changed */
      unsigned         mmap:1;
  
      unsigned         recycled:1;
      unsigned         in_file:1;
      unsigned         flush:1;
      unsigned         sync:1;
      unsigned         last_buf:1;
      unsigned         last_in_chain:1;
  
      unsigned         last_shadow:1;
      unsigned         temp_file:1;
  
      /* STUB */ int   num;
  };

##ngx_list_t ngxin中的list数据结构
  src/core/ngx_list.h|c

  //list结构
  typedef struct {
	ngx_list_part_t *last; //指向该链表的最后一个节点
	ngx_list_part_t  part; //头结点
	size_t			size;  //具体元素所需要的内存大小;?每个,还是总共
	ngx_unit_t		nalloc;//每个节点可包含的具体元素的个数
	ngx_pool_t		*pool; //该list使用的分配内存的pool
  } ngx_list_t;

  //list结构中的节点
  typedef struct ngx_list_part_s ngx_list_part_t;
  struct ngx_list_part_s{
	void			*elts; //每个节点存放具体元素的开始地址,可以理解为一个数组(elts[])
	ngx_unit_t		nelts; //该节点包含的元素的个数
	ngx_list_part_t *next; //该节点指向的下一个节点
  } 

##ngx_queue_t ngxin中的双向链表
  src/core/ngx_queue.h|c

  typedef struct ngx_queue_s ngx_queue_t;
  struct ngx_queue_s {
	ngx_queue_t		*prev;
	ngx_queue_t		*next;
  };

  可以看到这个双向链表没有,节点中没有数据成员,只有前一个和后一个,在使用的时候需要
  先定义个哨兵：
    ngx_queue_t free;  //哨兵,不放任何数据,prev指向链表头,next指向第一个节点

  使用一个具有数据元素的链表节点时,只要在相应的结构体上加一个ngx_queue_t成员就可以。
  比如:
    typedef struct { //加上一个ngx_queue_t后,这就是一个该queue的具体数据
		int 			a;
		ngx_queue_t		queue; //代表一个链表节点
		char			*name;
    } mydata;
  当我们知道 ngx_queue_t *q 指向的是链表中queue后,我们可以通过queue在mydata
  中的偏移量,来获取mydata的地址:
	1.获取queue在mydata结构体中的偏移量
	  size_t offset = offsetof(mydata,queue);	
 	2.让queue的具体地址减去偏移量,这个就是mydata的地址
	  mydata data = (mydata)((char *)q - offset);	

  
