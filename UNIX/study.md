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
  //返回值,出错返回负一
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
  * O_CREAT	弱此文件不存在则创建它。使用该选项时必须设置mode_t参数指定权限
  * O_DIRECTORY 如果指定的不是目录,则出错
  * O_EXCL	如果同时指定了O_CREAT,而文件存在,则出错 
  * O_NOCTTY	如果path引用的是终端设备,则不将该设备分配做为进程控制终端
  * O_NOFOLLOW  如果path引用的是一个符号连接,则出错
  * O_NONBLOCK	设置I/O操作为非阻塞方式
  * O_SYNC	同步write操作 //TODO ? 和上面的参数是不是不能同时使用
  * O_TRUNC	如果文件存在,并且为只写或读写方式打开,则将其长度截断为0
  * O_RSYNC	同步read操作,直至所有对文件同一部分挂起的写操作都完成 //?同一部分是啥意思。
		另外Linux3.2在处理该标记和O_SYNC时相同

##creat(创建一个新文件)
  ```c
     #include <fcnt1.h>
 
     int creat(const char *path, mode_t mode);
     //返回值:成功返回只写打开的文件描述符;失败,返回-1
  ```
  此函数和带下面参数的open函数等效

  ```c
     open(path, O_WRONLY|O_CREAT|O_TRUNC, mode);
  ```
  
##close
  关闭一个打开的文件
  ```c
    #include<unistd.h>
    
    int close(int fd);
    //返回值: 成功,返回0; 出错,返回-1
  ```
  关闭一个文件时同时会释放该进程在该文件上的所有记录锁。
  另外当一个进程终止时，内核自动关闭头所有的打开文件。

##lseek
  设置文件偏移量
  ```c
     #include<unistd.h>
    
     off_t lseek(int fd, off_t offset, int whence);
     //返回值: 若成功,返回文件的新偏移量; 若出错,返回-1
  ```
  * offset	偏移值
   * 当whence参数为SEEK_SET时,则以文件的偏移地址设置为文件的开始位置处加上offset个字节  
   * 当whence为SEEK_CUR时,则以文件的当前地址加offset字节为偏移地址,offset可为正负数
   * 当whence为SEEK_END时,则文件的偏移地址为文件的长度加上offset个字节

  如果一个文件描述符指向的是一个管道、FIFO或网络套接字,则lseek返回-1，并经errno设置为ESPIPE

##函数read
  ```c
    #include <unistd.h>

    ssize_t read(int fd, void *buf, size_t nbytes);
    //返回值: 读到的字节数,若已到文件尾,返回0; 若出错,返回-1
   
    //其中ssize_t(有符号类型),size_t(无符号类型)是在1990 POSIX.1标准中引入的
  ``` 
  * fd		文件描述符
  * buf		读到的数据
  * nbytes	打算读的字节数

##函数write
  ```c
    #include <unistd.h>

    ssize_t write(int fd, const void *buf, size_t nbytes);
    //返回值: 若成功,返回已写的字节数; 若出错,返回-1
  ```
  * fd		文件描述符
  * buf		将要写入的数据
  * nbytes	打算写入的数据
 
##Unix用三种数据结构表示打开的文件 
  * 描述符表(每个进程一个)
   * fd号和文件指针(指向文件表项)
 
  * 文件表项(每个fd一个,也可以多个fd一个)
   * 文件状态标志(读、写、非阻塞等)
   * 文件偏移量
   * v_node节点指针

  * v_node节点表项(代表一个物理文件,所有进程共享)
   * 节点信息
   * 文件大小
   * 文件类型等

##函数dup、dup2
  复制一个现有的文件描述符 
  ```c
    #include <unistd.h>
 
    int dup(int fd);

    int dup2(int fd, int fd2);

    //返回值: 若成功,返回新文件的描述符; 若出错,返回-1
  ```
  这个函数返回的新文件描述符一定是当前可用文件描述符中最小的值。
  返回的新的文件描述符和老fd描述符指向同一个文件表项。
 
  * fd2 指定新描述符的值
    > 如果fd2已经打开,则先将其关闭。如果fd和fd2相等,则返回fd2,不关闭。
    > 如果指定的fd2没有打开,则打开它,并且fd2的FD_CLOEXEC标志被清除
    > FD_CLOEXEC表示在当前进程调用exec生成新的进程时,所有从父进程继承的描述符都将被关闭。
    > 调用fork生成的进程不会。
    > 如果是用dup2把fd2的FD_CLOEXEC清除掉后,exec生成的新进程就不会关闭继承的描述符。

##函数sync、fsync和fdatasync
  ```c
    #include<unistd.h>
	
    int fsync(int fd);
    
    int fdatasync(int fd);
    //以上两个函数返回值: 若成功,返回0; 若出错,返回-1

    void sync(void);
  ```
  sync函数对所有fd起作用(?是当前进程还是所有进程),负责把用户态的数据复制到内核,然后返回,
  它不保证数据实际写入磁盘。
  fsync对指定文件起作用,并且等待文件写入磁盘。
  fdatasync只对指定文件的数据部分起作用,保证文件数据写入磁盘,文件其它属性不管。

##函数fcntl
  可以改变或获取已打开文件的属性
  ```c
    #include<fcntl.h>
	
    int fcntl(int fd, int cmd, ... /* int arg */);
    //返回值: 若成功,则依赖cmd的值; 若出错,返回-1
  ```
  cmd有如下值:
  * F_DUPFD	复制文件描述符和dup函数类似。新文件描述符是返回值,但是
		有自己的一套文件描述符标志,FD_CLOEXEC标记被清除,表示在exec时
		新描述符不会被关闭。




