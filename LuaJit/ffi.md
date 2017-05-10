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

