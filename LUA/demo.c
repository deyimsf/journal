#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
//MAC下编译有问题
//在测试机上需要正确的引用lua.h等文件
//学习下如果引用头文件
//学习下Makefile

static int msg(lua_State *L){
	//检查lua函数的第一个参数是不是数字,如果是则返回这个数字
    //如果发生错误则这个lua函数会抛出异常,并返回错误信息
    //(例如:bad argument #1). 
	//lua_Number是一个类型定义,如下：
    //typedef double lua_Number;
    //可以在luaconf.h中修改
	lua_Number num = luaL_checknumber(L,1);
	
    //将数字原样放入返回值中
    lua_pushnumber(L,num); //第一个返回值
	//返回信息
    lua_pushstring(L,"sucess"); //第二个返回值

	//该函数有两个返回值
	return 2;
}


//将这个c函数注册到lua中
//一个luaopen_*(*表示c库的名称)函数指定C库的名称
//lua通过require等关键字可以直接调用luaopen_*这个函数
int luaopen_demo(lua_State *L){
	//所有要注册到lua中的c函数放到luaL_Reg数组中
    const luaL_Reg methods = {
				{"msg",msg},
				{NULL,NULL}		
	};

	//注册c方法到lua
    //此时栈顶的值是demo
	luaL_register(L,"demo",methods);
	
	//设置demo.null的值
	lua_pushlightuserdata(L,NULL); //此时栈顶元素是NULL
	//将栈顶的值,放置到栈顶第二个位置的null属性中,然后弹出栈顶
	//demo.null=NULL
	lua_setfield(L,-2,"null"); 

    //设置版本
    lua_pushliteral(L,"0.0.1");
	//demo.version=0.0.1
	lua_setfield(L,-2,"version");

	return 1;
}



