#时间值
unix为每个进程维护了三个时间值

  * 时钟时间(wall clock time)
  * 用户CPU时间
  * 系统CPU时间

时钟时间是进程运行的总时间(从开始到结束)。
用户CPU时间是执行用户指令cpu所有的时间,不包括阻塞时间。
系统CPU时间是系统执行系统调用在内核所花费的时间。 

命令time可以度量出某个程序的这三个时间,例如:

```bash
 $ time curl http://www.jd.com  > /dev/null
 real	0m0.344s
 user	0m0.009s
 sys	0m0.007s
```

其中real是curl从开始到结束运行的总时间,user是curl程序执行非内核程序使用的时间。
sys是curl调用内核函数执行的时间。
sys+user表示curl在这次执行中总共使用的cpu时间(不包括阻塞时间)。


#文件I/O
这里介绍的I/O被称为不带缓冲的I/O(unbuffered I/O)。
这些I/O函数不是ISO C的组成部分,是POSIX.1和Single UNIX Specification的组成部分。

##open
```c
  #include <fcnt1.h> //oflag参数的常量定义在该头文件中
  
  int open(const char *path, int oflag, ... /* mode_t mode //比如文件权限等 */)
```
 * path 要打开的文件或目录名字 
 * oflag常量
  * O_RDONLY	只读打开 
  * O_WRONLY	只写打开
  * O_RDWR	读、写打开
  * O_EXEC	只执行打开
  * O_SEARCH	值搜索打开(应用于目录,不是所有的系统都支持该标记)

   > 以上必须指定,且只能指定一个
  * O_APPEND	




