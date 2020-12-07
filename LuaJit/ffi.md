##ffi词汇
* cdecl: 抽象C类型声明(lua字符串)
* ctype: C类型(如int,char等),服务于ffi.typeof()方法。
* cdata: C数据对象(如int a中的a),他是ctype类型的。
* ct: 一个c类型描述符,比如 "struct person"(如ffi.typeof("struct person"))


##使用举例
```lua
	ffi = require "ffi";
	
	ffi.cdef[[
		typedef struct {
			int age;
			int height;
		} person_t;
	]]
```

##一般类型
* lua形式:
```lua
	-- 直接用ct创建一个cdata
	local person = ffi.new("person_t");

	-- 使用ctype创建cdata
	local person_type = ffi.typeof("person_t")
	local person = ffi.new(person_typ);
```
* c类型:
```lua
	person_t person;
```

##指针类型
* lua形式:
```c
	local person_ptr = ffi.new("person_t *");

	local person_ptr_type = ffi.typeof("person_t *");
	local person_ptr = ffi.new(person_ptr_type);
```
* c形式:
```c
	person_t *person_ptr;
```


##数组形式:
* lua形式:
```lua
	local person_arr = ffi.new("person_t[?]",5);
 	local person_arr = ffi.new("person_t[256]");
	local person_arr = ffi.new("person_t[1]",{ {1;2 } });
	local person_arr = ffi.new("person_t[1]",{ {height=1;age=2} });

	local person_arr_type  = ffi.typeof("person_t[?]");
    local person_arr = ffi.new(person_arr_type,5);

    local person_arr_type = ffi.typeof("person_t[5]");
    local person_arr = ffi.new(person_arr_type);
```
* c形式:
```c	
	person_t person_arr[5];
   	person_t person_arr[1] = {1;2}
```

##ffi.cast(ct, init)
* 用ct创建一个cdata数据,cdata的值用init初始化
* 另一种理解,把init强转成cdata数据:
```lua
	local str = "hello";
    local c_str = ffi.cast("char *", str);
    print(ffi.string(c_str));
```


##lua中的字符串地址赋值给一个c变量
```c
	char ***str; //对这个str赋值如下
	
	char *a = "hello";
	char *b = &a;
	char *c = &b;	
	
	str = c; //搞定
```
上面的代码在lua中有如下对应:
```lua
	local str = "hello"; -- 一个lua字符串
	
	// 加1是因为c语言里面需要一个'\0'作为字符串的结尾
	// 如果第一个参数是VLA或VLS,则第二个参数必须指定个数,第三个参数随意
	// 比如"char [?]" 是一个VLA(变长数组)
	// 比如"struct person[?]" 是一个VLS(变长结构体)
	local c_str_a = ffi.new("char [?]",string.len(str)+1, str);
    
    // 在C中类似 char *b[3] = &a;
    // 数组里面的值b[i]是一个指针,这个指针(b[i])指向一个char型
    // ffi.new中的语法类似把正常的c变量定义中的变量拿掉了 
    // 比如: int *a[3], 可以写成ffi.new("int *[3]")
	local c_str_b = ffi.new("char *[1]", c_str_a); 	
    local c_str_c = ffi.new("char *[1]", c_str_b);

	-- 其中 c_str_c 就对应了c中的 ***str
```

##在lua中声明一个指针类型的变量并赋值(如int *c_int_a = 15):
```lua
	local c_int_a = ffi.new("int[1]", 15);
	print(c_int_a[0]);
```
上面的代码在c语言中有如下对应:
```c
   int c_int_a[1] = 15;
```

##luajit中,无法直接将一个lua数据赋值给指针,可以这样做
```lua
   local tmp = "hello";
   local str_a = ffi.new("char [?]", string.len(str_a)+1, str_a);
   local str_b = ffi.new("char *",str_a); 
   print(tostring(str_b));
   -- 其实str_a和str_b是一样的，都是一个指针，都是指向“hello”这个字符串

   local tmp = 15;
   local int_a = ffi.new("int [1]", 15);
   local int_b = ffi.new("int *", int_a);
   print(int_b[0]);

```


## 将一个指针表示的字符串打印
str = ffi.string(ptr [,len])


## 举例打印系统毫秒时间
* 获取系统时间的函数声明
	int gettimeofday(struct timeval *tv, void *tz);

* 用C实现如下:
```c
	#include <stdio.h>
	#include <sys/time.h>

	int main(){
        	struct timeval time;
        	int status = gettimeofday(&time,NULL);
    
        	if(status < 0){ 
                	return -1; 
         	}   
    
         	long millis = (long)time.tv_sec * 1000 + (long)time.tv_usec / 1000;
    
         	printf("%lu\n",millis);
	} 
```

* 另一种用C实现的方式:
```c
        #include <stdio.h>
        #include <sys/time.h>

        int main(){
                struct timeval time[1];
                int status = gettimeofday(time,NULL);
    
                if(status < 0){ 
                        return -1; 
                }   
    
                long millis = (long)time[0].tv_sec * 1000 + (long)time[0].tv_usec / 1000;
    
                printf("%lu\n",millis);
        }   
```

可以看到以上两种方式在传第一个入参的时候,一个是&time,一个是time,其实都是传的地址,但是这个地址
指向的内存(结构体)需要我们事先分配好,这两种方式都是实现分配好的。

* 用ffi实现如下
```lua
	local ffi = require("ffi");

	ffi.cdef[[
    		/********************时间函数声明********************/
		struct timeval{
			long int tv_sec;
			long int tv_usec;
		};

		int gettimeofday(struct timeval *tv, void *tz);
	]]

	local function time(){
		-- 这一句类似C中的 struct timeval time[1];
        	local c_timeval = ffi.new("struct timeval[1]");
		-- 这里的c_timeval就相当于C中的 time变量了,如此就可以清晰的看出我们传的是地址了
        	ffi.C.gettimeofday(c_timeval,NULL);

        	local current_time = c_timeval.tv_sec * 1000 + c_timeval.tv_usec / 1000;
		
		return current_time;
	}	
```

* 上面的例子是针对C函数要求一个一级指针传参,如果需要一个二级指针传参怎么办?
```

   // 一级指针
	timeval *tv
	 -----
	 | * |
	 -----
	   \
	   -----------
	   | timeval |
	   -----------

   // 二级指针
	timeval **tv
	 -----
	 | * |
	 -----
	   \
	   -----
	   | * |
	   -----
	      \
	     -----------
	     | timeval |
	     -----------

   // 用C写可以这样
	timeval time[1];
	timeval *a_time[1];	
	
	a_time[0] = time;
   // 这样a_time在内存中的结构就和 **tv是一样的了
```

所以用ffi的时候,如果遇见多层次的指针传递(最后的结构体需要自行分配内存的),就可以仿效上面的方式:
```lua
	// 相当于C的 struct timeval time[1]
    local c_timeval = ffi.new("struct timeval[1]");

	// 相当于C的 struct timeval *a_time[1];  a_time[0] = time; 这两句。
	// 最后C中的a_time和ffi中的a_c_timeval变量的内存结构一致
	local a_c_timeval = ffi.new("struct *timeval[1]", c_timeval);
```

注意:
    在ffi中用一个长度是1的数组来获取变量的地址,是因为ffi中没有获取地址的修饰符(C中是&)


