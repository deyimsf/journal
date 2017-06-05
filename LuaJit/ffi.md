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

	local person_ptr_type = ffi.typeof("person_t*");
	local person_ptr = ffi.new(person_ptr_type);
```
* c形式:
```c
	person_t *person;
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

    	local person_arr_type = ffi.typeof("person_t[8]");
    	local person_arr = ffi.new(person_arr_type);
```
* c形式:
```c	
	person_t arr[5];
   	person_t arr[1] = {1;2}
```

##ffi.cast(ct, init)
* 用ct创建一个cdata数据,cdata的值用init初始化
* 另一种理解,把init强转成cdata数据:
```lua
	local str = "hello";
    	local c_str = ffi.cast("char *", str);
    	print(c_str[0]);
    	print(c_str[1]);
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
	local c_str_a = ffi.new("char [?]",string.len(str)+1, str);
	local c_str_b = ffi.new("char *[1]", c_str_a);
	local c_str_c = ffi.new("char *[1]", c_str_b);

	-- 其中 c_str_c 就对应了c中的 ***str
```

在lua中声明一个指针类型的变量并赋值(如int *c_int_a = 15):
```lua
	local c_int_a = ffi.new("int[1]", 15);
	print(c_int_a[0]);
```



