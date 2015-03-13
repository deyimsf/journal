#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

typedef unsigned long long uint64_t;
typedef long long int64_t;
typedef long int int32_t;

static uint64_t murmurhash64A(const void *key,size_t len,int32_t seed);
static void setfuncs (lua_State *l, const luaL_Reg *reg, int nup);

//mod(key,num) 取摸
static int mod(lua_State *L){
	//checks whether the key is a string
	size_t len;
	const void *key = luaL_checklstring(L,1,&len);
	//checks if the num is a number
	int num = luaL_checknumber(L,2);
	
	uint64_t seed=0x1234ABCD;	
	uint64_t h = murmurhash64A(key,len,seed);	
	
	h = h & 0x7FFFFFFFFFFFFFFF;
	int index = h%num;
	lua_pushnumber(L,index); //the first return value
	
	//there is one return value	
	return 1;
}


//将这个c函数注册到lua中
int luaopen_chash(lua_State *L){

	//lua_register(L,"mod",mod);

	//所有要注册到lua中的c函数放到luaL_Reg数组中
	const luaL_Reg methods[] = {
						{"mod",mod},
						{NULL,NULL}
	};

	//注册c方法到lua
	//此时栈顶的值是chash
	////luaL_register(L,"chash",methods);
	lua_newtable(L);
    setfuncs(L,methods,0);	

	//设置chash.null的值
	lua_pushlightuserdata(L,NULL); //此时栈顶元素是NULL
	//将栈顶的值,放置到栈顶第二个位置的null属性中,然后弹出栈顶
	//chash.null=NULL
	lua_setfield(L,-2,"null"); 

	//设置版本
	lua_pushliteral(L,"0.0.1");
	//demo.version=0.0.1
	lua_setfield(L,-2,"version");

    //返回栈顶的表	
	return 1;
}

/**
 * 把数组reg中所有的函数注册到栈顶的表中
 * 并指定upvalue的个数   lua 5.2中的代码
 */
static void setfuncs (lua_State *l, const luaL_Reg *reg, int nup){
     int i; 

     luaL_checkstack(l, nup, "too many upvalues");
     for (; reg->name != NULL; reg++) {  /* fill the table with given functions */
         for (i = 0; i < nup; i++)  /* copy upvalues to the top */
             lua_pushvalue(l, -nup);
         lua_pushcclosure(l, reg->func, nup);  /* closure with those upvalues */
         lua_setfield(l, -(nup + 2), reg->name);
     }
     lua_pop(l, nup);  /* remove upvalues */
}


//----murmurhash64A
static uint64_t murmurhash64A(const void *key,size_t len,int32_t seed){

	int64_t m = 0xc6a4a7935bd1e995LL;
	int r = 47;

	//h声明为无符号类型,后续可实现无符号右移	
	uint64_t h = seed ^ (len * m);

	const uint64_t * data = (const uint64_t *)key;
	const uint64_t * end = data + (len >> 3);

	while(data != end){
	    	uint64_t k = *data++;

    		k *= m; 
    		k ^= k >> r; 
    		k *= m; 
	
			h ^= k;
    		h *= m; 
	}
	
	const unsigned char * data2 = (const unsigned char *)data;
	switch(len & 7){
  		case 7: h ^= (uint64_t)data2[6] << 48;
  		case 6: h ^= (uint64_t)data2[5] << 40;
  		case 5: h ^= (uint64_t)data2[4] << 32;
  		case 4: h ^= (uint64_t)data2[3] << 24;
  		case 3: h ^= (uint64_t)data2[2] << 16;
  		case 2: h ^= (uint64_t)data2[1] << 8;
  		case 1: h ^= (uint64_t)data2[0];
          		h *= m;
 	};
 
	h ^= h >> r;
	h *= m;
	h ^= h >> r; 

	return h; 
}

