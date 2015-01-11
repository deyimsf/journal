#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

typedef unsigned long long uint64_t;


//mod(key,num) 取摸
static int mod(lua_State *L){
    //checks whether the key is a string
    size_t len;
	const char *key = luaL_checklstring(L,1,&len);
	//checks if the num is a number
    int num = luaL_checknumber(L,2);
	
	uint64_t seed=0x1234ABCD;	
	//-------murmurHash
	const uint64_t m = 0xc6a4a7935bd1e995LLU;
	const int r = 47;
	
	uint64_t h = seed ^ (len * m);

	const uint64_t * data = (const uint64_t *)key;
	const uint64_t * end = (len >> 3) + data;

	while(data != end){
	    	uint64_t k = *data++;

    		k *= m; 
    		k ^= k >> r; 
    		k *= m; 
    		h *= m; 
	}

  	const unsigned char * data2 = (const unsigned char *)data;

  	switch(len & 7){
  		case 7: h ^= (uint64_t)(data2[6]) << 48;
  		case 6: h ^= (uint64_t)(data2[5]) << 40;
  		case 5: h ^= (uint64_t)(data2[4]) << 32;
  		case 4: h ^= (uint64_t)(data2[3]) << 24;
  		case 3: h ^= (uint64_t)(data2[2]) << 16;
  		case 2: h ^= (uint64_t)(data2[1]) << 8;
  		case 1: h ^= (uint64_t)(data2[0]);
          		h *= m;
  	};
 
	h ^= h >> r;
	h *= m;
	h ^= h >> r;  

	h = h & 0x7FFFFFFFFFFFFFFF;
	int index = h%num;
    //将摸值放入到栈中
    lua_pushnumber(L,index); //第一个返回值
		
	//该函数有一个返回值
	return 1;
}


//将这个c函数注册到lua中
//一个luaopen_*(*表示c库的名称)函数指定C库的名称
//lua通过require等关键字可以直接调用luaopen_*这个函数
int luaopen_chash(lua_State *L){
	//所有要注册到lua中的c函数放到luaL_Reg数组中
    	const luaL_Reg methods[] = {
					{"mod",mod},
					{NULL,NULL}
				};

	//注册c方法到lua
    //此时栈顶的值是chash
	luaL_register(L,"chash",methods);
	
	//设置chash.null的值
	lua_pushlightuserdata(L,NULL); //此时栈顶元素是NULL
	//将栈顶的值,放置到栈顶第二个位置的null属性中,然后弹出栈顶
	//chash.null=NULL
	lua_setfield(L,-2,"null"); 

    //设置版本
    lua_pushliteral(L,"0.0.1");
	//demo.version=0.0.1
	lua_setfield(L,-2,"version");

	return 1;
}



