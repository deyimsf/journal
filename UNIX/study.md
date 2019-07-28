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

###open
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
  * O_SYNC	同步write操作到磁盘,数据库程序一般使用该标记(Linux似乎对该标记不感冒)
  * O_TRUNC	如果文件存在,并且为只写或读写方式打开,则将其长度截断为0
  * O_RSYNC	同步read操作,直至所有对文件同一部分挂起的写操作都完成 //?同一部分是啥意思。
		另外Linux3.2在处理该标记和O_SYNC时相同

###creat(创建一个新文件)
```c
  #include <fcnt1.h>
 
  int creat(const char *path, mode_t mode);
  //返回值:成功返回只写打开的文件描述符;失败,返回-1
```
此函数和带下面参数的open函数等效

```c
  open(path, O_WRONLY|O_CREAT|O_TRUNC, mode);
```
  
###close
关闭一个打开的文件
```c
  #include<unistd.h>
    
  int close(int fd);
  //返回值: 成功,返回0; 出错,返回-1
```
关闭一个文件时同时会释放该进程在该文件上的所有记录锁。
另外当一个进程终止时，内核自动关闭头所有的打开文件。

###lseek
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

###函数read
```c
  #include <unistd.h>

  ssize_t read(int fd, void *buf, size_t nbytes);
  //返回值: 读到的字节数,若已到文件尾,返回0; 若出错,返回-1
   
  //其中ssize_t(有符号类型),size_t(无符号类型)是在1990 POSIX.1标准中引入的
``` 
 * fd		文件描述符
 * buf		读到的数据
 * nbytes	打算读的字节数

###函数write
```c
  #include <unistd.h>

  ssize_t write(int fd, const void *buf, size_t nbytes);
  //返回值: 若成功,返回已写的字节数; 若出错,返回-1
```
 * fd		文件描述符
 * buf		将要写入的数据
 * nbytes	打算写入的数据
 
###Unix用三种数据结构表示打开的文件 
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

###函数dup、dup2
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

###函数sync、fsync和fdatasync
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

###函数fcntl
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
  * F_DUPFD_CLOEXEC	同上,不同的是新描述符会被设置FD_CLOEXEC标记 
  * F_GETFD	获取fd的文件描述符标志，当前只定义了一个文件描述符标记,即FD_CLOEXEC 
  * F_SETFD	设置fd的文件描述符标志。0(系统默认,exec时不关闭)或1(在exec时关闭)
  * F_GETFL	获取fd的文件状态标志(O_RDONLY、O_WRONLY等),需用返回值和O_ACCMODE按位与
		然后在跟O_RDONLY等标记进行对比		
  * F_SETFL	设置fd的文件状态标志(O_APPEND、O_NONBLOCK、O_SYNC等)
  * F_GETOWN	获取当前SIGIO和SIGURG信号的进程ID(正数代表)或进程组ID(负数代表)
  * F_SETOWN	设置接收SIGIO和SIGURG信号的进程ID或进程组ID


#文件和目录

###获取文件和目录信息的函数
```c
  #include <sys/stat.h>
   
  int stat(const char *filepath, struct stat *buf);

  int fstat(int fd, struct stat *buf);

  int lstat(const char *filepath, struct stat *buf);
  int fstat(int fd, const char *filepath, struct stat *buf);

  //返回值: 若成功,返回0, 若出错,返回-1
```
 * stat函数
   * filepath	文件路径
   * buf	返回的文件信息

 * fstat函数
   * fd		文件描述符
   * buf	返回的文件的信息

其中,struct stat结构如下:
```c
   struct stat {
   	dev_t		st_dev;		/*文件设备编号*/
	ino_t		st_ino;		/*i-node节点号*/
	mode_t		st_mode; 	/*文件的类型和存取权限*/
	nlink_t		st_nlink;	/*到该文件的硬链接数目*/
	uid_t		st_uid;		
	gid_t		st_gid;
	dev_t		st_rdev;
	off_t		st_size;	/*文件大小(字节)*/
	struct timespec	st_atime;
	struct timespec st_mtime;
	struct timespec st_ctime;
	blksize_t	st_blksize;   /*文件系统I/O缓冲区大小*/
	blkcnt_t	st_blocks;    /*磁盘块数*/
   };
```

###测试访问权限
```c
   #include <unistd.h>
  
   int access(const char *pathname, int mode);
  
   int faccessat(int fd, const char *pathname, int mode, int flag);
   //成功返回0,错误返回-1
```
 * pathname	要测试的文件路径名
 * mode		
   * R_OK	测试读权限
   * W_OK	测试写权限
   * X_OK	测试执行权限

###umask
为当前进程设置,文件创建时不能具有的权限(如，用户读,组用户写)。
```c
    #include <sys/stat.h>
    
    mode_t umask(mode_t cmask);
    // 返回值是之前进程创建文件时,不能具有的权限位
```
  * cmask	要屏蔽的权限
    * 0 400	用户读 (r-- --- ---)
    * 0 200	用户写 (-w- --- ---)
    * 0 100	用户执行(--x --- ---)
    * 0 040	组读(--- r-- ---)
    * 0 020     组写(--- -w- ---)
     ...
    * 0 001	其他执行(--- --- --x) 

```bash
  // 打印当前进程默认创建新文件时屏蔽的权限
  $ umask
  0022

  // 打印符号形式的屏蔽权限
  $ umask -S
  u=rwx,g=rx,o=rx
```
  
###改变文件的访问权限
```c
   #include <sys/stat.h>
 
   int chmod(const char *pathname, mode_t mode);
   int fchmod(int fd, mode_t mode);
   int fchmodat(int fd, const char *pathname, mode_t mode, int flag);
   //成功返回0, 出错返回-1
```

###更改用户id和组id
```c
   #include <unistd.h>

   int chown(const char *pathname, uid_t owner, gid_t group);
   int fchown(int fd, uid_t owner, git_t group);
   //成功返回0,错误返回-1
```

###文件截断
```c
   #include <unistd.h>

   int truncate(const char *pathname, off_t length);
   int ftruncate(int fd, off_t length);
   //成功返回0,出错返回-1
```

###硬链接和软连接
硬链接表示代表当前文件的i节点,指向的内容就是文件的实际磁盘内容。
软链接所代表的i节点,指向的内容是某个实际文件的路径名字。

###函数link,linkat
```c
   #include <unistd.h>
	
   int link(const char *existingpath, const char *newpath);
   int linkat(int efd, const char *existingpath, int nfd, const char *newpath, int flag);
   //成功返回0,出错返回-1
```
为现有的文件existingpath创建一个硬链接。

###函数unlink,unlinkat,remove
```c
   #include <unistd.h>

   int unlink(const char *pathname);
   int unlinkat(int fd, const char *pathname, int flag);
   // 成功返回0，失败返回-1
```
这两个函数用来删除目录项(并不是删除磁盘文件),并对所引用文件的链接计数器减1。
如果该文件还有其他链接,则仍可通过其他连接访问该文件的数据。
如果最后没有任何链接指向该文件,那么该文件所占的空间就可以被随意使用了。

```c
   #include <stdio.h>
	
   int remove(const char *pathname);
```
如果pathname是文件则调用unlink函数，如果是目录则调用rmdir函数。

###目录或文件重命名
```c
   #include <stdio.h>

   int rename(const char *oldname, const char *newname);
   int renameat(int oldfd, const char *oldname, int newefd, const char *newname);
   //成功返回0,出错返回-1
```

###创建和读取符号链接
创建符号链接
```c
    #include <unistd.h>

    int symlink(const char *actualpath, const char *sympath);
    int symlinkat(const char *actualpath, int fd, const char *sympath);
    //成功返回0,出错返回-1
``` 
 
读取符号链接中的内容
```c
    #include <unistd.h>

    ssize_t readlink(const char *pathname, char *buf, size_t bufsize);
    ssize_t readlinkat(int fd, const char *pathname, char *buf, size_t bufsize);
    //成功返回读取的字节数,出错返回-1
```

###创建目录和删除目录
创建目录
```c
    #include <sys/stat.h>

    int mkdir(const char *pathname, mode_t mode);
    int mkdirat(int fd, const char *pathname, mode_t mode);
    //成功返回0，出错返回-1
```

删除目录
```c
    #include <unistd.h>
   
    int rmdir(const char *pathname);
    //成功返回0，出错返回-1
```

###读目录
```c
   #include <dirent.h>

   // 打开一个目录,返回该目录的句柄
   // 成功则返回指针，错误返回NULL
   DIR *opendir(const char *pathname);
     
   // 读取目录中的内容
   // 成功则返回指针,错误返回NULL
   struct dirent *readdir(DIR *dp);

   // 关闭打开的目录流
   // 成功返回0,错误返回-1
   int closedir(DIR *dp);
```

其中struct dirent结构如下(操作系统不一样,该结构也会有差异):
```c
   struct dirent {
	ino_t d_ino;			/* file number of entry */
	__uint16_t d_reclen;		/* length of this record */
	__uint8_t  d_type; 		/* file type, see below */
	__uint8_t  d_namlen;		/* length of string in d_name */
	char d_name[__DARWIN_MAXNAMLEN + 1];	/* name must be no longer than this */
   };
 ```
 * d_ino  文件或目录的i_node号
 * d_name 文件或目录名字


#文件锁
###建议性锁
不具备强制性。比如一个进程将某个文件锁住,另一个进程可以对该文件进行修改操作。 
就想红路灯就是建议行锁,红灯只是建议不能通过,但是没办法强制你不通过。
所以如果大家都遵守交通负责的话,多个进程仍然可以协同工作。

###强制性锁
具有强制性。比如一个进程锁定某人文件后,另一个进程无法对其进行修改操作。

###共享锁
也可以叫,读锁。
当某个进程获取到共享锁后,其他进程也可以获取这个锁,但任何进程都不能再获取它的写锁。
只有所有进程都释放共享锁后才能对其加写锁。 

###排他锁
也可以叫,写锁。
该锁有排他性,只能被一个进程获取。

###flock()函数对文件加锁解锁
这是一个建议性锁,并且锁住的是整个文件,无法锁定某一区域。
```c
    #include <sys/file.h>
   
    int flock(int fd, int operation);
    // 成功返回0,错误返回-1  
```
operation参数设定值如下:
  * LOCK_SH  获取共享锁
  * LOCK_EX  获取互斥锁(写锁) 
  * LOCK_NU  解锁
  * LOCK_NB  当文件已经被另一个进程获取写锁时,不阻塞该操作
   
    > flock(fd, LOCK_EX),当fd被其他进程锁住时,当前进程阻塞；
    > ret = flock(fd, LOCK_EX|LOCK_NB),当fd已经被其他进程锁住时,返回错误,
    > 同时ret = -1, errno = EWOULDBLOCK。

###lockf文件区域锁
可以对文件的某个区域进行加锁,仍然是建议性锁。
```c
    #include <unistd.h>
  
    int lockf(int fd, int cmd, off_t len);
    //成功返回0，错误返回-1并设置errno值
  ``` 
cmd参数值如下:
  * F_ULOCK   解锁
  * F_LOCK    写锁(阻塞)
  * F_TLOCK   写锁(非阻塞,[EAGIN]错误)
  * F_TEST    如果被锁,返回-1;否则返回0 
  
len是要锁定或解锁的连续字节数


#标准I/O库
###打开流
```c
   #include <stdio.h>

   FILE *fopen(const char *pathname, const char *type);
   FILE *freopen(const char *pathname, const char *type, FILE *fp);
   FILE *fdopen(int fd, const char *type);
   //若成功,返回文件指针; 若出错,返回-1
```

###格式化I/O
```c
   #include <stdio.h>
   
   // 输出到标准输入流
   int printf(const char *format, ...);
   
   // 输出到fp中
   int fprintf(FILE *fp, const char *format, ...);
   
   // 输出到fd中
   int dprintf(int fd, const char *format, ...);

   //以上三个函数,成功则返回输出字符数;出错则返回负值

   int sprintf(char *buf, const char *format, ...);
   // 成功返回存入到buf中的字符数; 出错则返回负值

   int snprintf(char *buf, size_t n, const char *format, ...);
   // 若缓冲区足够大,返回将要存入buf的字数数; 出错返回负值
```


#系统数据文件和信息
###系统标识
```c
   #include <sys/utsname.h>

   int uname(struct utsname *name);
   // 若成功,返回非负值; 若出错,返回-1
```

该函数负责填充的struct utsname结构体如下:
```c
   struct utsname {
	char sysname[];
	char nodename[];
	char release[];
	char version[];
	char machine[];
   }
```

###时间和日期
返回当前时间和日期
```c
   #include <time.h>
   
   time_t time(time_t *calptr);
   // 若成功则返回时间值;失败则返回-1
```

如果calptr参数不为NULL,则时间也放在calptr指向的结构体内。

clock_gettime函数可以获取指定时钟的时间。(?什么是指定时钟)
```c
   #include <sys/time.h>

   int clock_gettime(clockid_t clock_id, struct timespec *tsp);
   // 成功则返回0,出错返回-1
```

当时钟id(clock_id)设置为CLOCK_PEALTIME时该函数和time函数相似,只是存
放时间的结构体不一样。

可以获得微妙精度的函数
```c
   #include <sys/time.h>
  
   int gettimeofday(struct timeval *tp, void *tzp);
   // 总是返回0 
```

 * tp  返回的时间值
 * tzp 只能是NULL

一般我们获取到特定的时间秒数后,需要通过某些函数,将其转换为分解的时间
结构,比如 struct tm结构体,然后调用另一个函数,生成人们可读的时间。

函数localtime和gmtime可以将time_t表示的时间,转换成分解的时间struct tm。
```c
   #include <time.h>
   
   struct tm *gmtime(const time_t *calptr);
   struct tm *localtime(const time_t *calptr);
   // 成功则返回分解的tm结构体;出错返回NULL
```

 * localtime 将日历时间转换成本地时区的时间
 * gmtime 将日历时间转换成统一的分解时间

函数strftime用来将分解的时间tm转换成人类可读的时间串。
```c
   #include <time.h>
 
   size_t strftime(char *buf, size_t maxsize, const char *format, 
		    const struct tm *tmptr);
   size_t strftime_t(char *buf, size_t maxsize, const char *format, 
		      const struct tm *tmptr, locale_t locale);
   // 返回存入到buf中的字符数; 否则返回0
```
 
一个例子如下:
```c
   #include <stdio.h>
   #include <stdlib.h>
   #include <time.h>

   int 
   main(void){
	time_t t;
   	struct tm *tmp;
 	char buf[64];

	// 获取本地时间
        time(&t);
	// 将本地时间time_t转换成分解的时间tm
	tmp = localtime(*t);

        // 使用strftime函数将tm转换成人类可读的时间串
	if (strftime(buf, 64, "time is: %r, %a %b %b %d, %Y", tmp) == 0) {
		printf("buf length 64 is too small\n");
	}else{
		printf("%s\n", buf);
	}

   }

```
结果输出 time is: 11:48:45 PM, Thu Jan 19, 2016

下面的函数和strltime相反,函数把一个人类可读的时间字符串,转换成分解的时间数据结构tm。
```c
   #include <time.h>
   
   char *strptime(const char *buf, const char *format, struct tm *tmptr);
   // 成功返回指针; 错误返回NULL
```


#进程环境

###进程退出
进程主要有以下方式终止程序
* 从main返回
* 调用exit
* 调用_exit或_Exit
* 调用abort异常终止
* 接到一个信号异常终止 

```c
 #include <stdlib.h>

 void exit(int status);
 void _Exit(int status);

 #include <unistd.h>
 void _exit(int status);
 
 //_Exit和_exit会直接进入内核去终止程序;exit会先执行一些清理程序,然后进入内核。
```

###atexit函数
该函数用来向进程注册清理程序,当进程显示或隐士的调用exit退出程序时,这些程序会被
exit执行,但如果是异常终止,或者时调用的_exit和_Exit则不会调用这些清理程序。

```c
 #include <stdlib.h>

 int atexit(void (*func)(void));
 //返回值; 若成功,返回0; 若出错,返回非0
```

###命令行参数
```c
int
main(int argc, char **argv){

}
```

ISO C和POSIX.1都要求argv[argc]是一个空指针。所以对参数的循环处理可以写成这样:
```c
for (i = 0; argv[i] != NULL; i++)
```

###环境表
每个进程都会接收到一个环境表。环境表和参数一样,是一个指针数组。全局变量environ
包含指针数组地址:
```c
extern char **environ;
```
		 环境指针	  环境表
		---------	---------
	environ	|	|-----> |	|-----> HOME=/home/sar\0
		---------	---------
				|	|----->PATH=:/bin:/usr/bin\0
				---------
				|	|----->SHELL=/bin/bash\0
				---------
				| NULL	|
				---------

例子：
```c
#include <stdio.h>

extern char **environ;

int 
main(int argc, char **argv){
        int i = 0;
        char *s; 
        while(environ[i] != '\0'){
                s = environ[i];
                ++i;
    
                printf("%s\n",s);    
        }    
}
```


###存储空间分配
ISO C阐述了3个用于存储空间动态分配的函数。
 * malloc 分配指定字节的存储区。存储区的初始值不确定。
 * calloc 分配指定个数指定大小的存储空间。初始化所有值为0。
 * realloc 重新分配存储空间。当增长时,可能需将以前分配的内容移到一个足够大的区域。

```c
 #include <stdlib.h>

 void *malloc(size_t size);
 void *calloc(size_t nobj, size_t size);
 void *realloc(void *ptr, size_t newsize);
 //以上函数若成功,返回非空指针; 若出错,返回NULL

 void free(void *ptr);
 //释放分配的空间
```


###环境变量
ISO C定义了一个函数来获取环境变量值
```c
 #include <stdlib.h>

 char *getenv(const char *name);
 //返回名字是name的环境变量的value; 若未找到,返回NULL
```

下面这几个函数来自POSIX.1和XSI
```c
 #include <stdlib.h>

 int putenv(char *str);
 	//若成功,返回0;若出错,返回非0

 int setenv(const char *name, const char *value, int rewrite);
 int unsetenv(const char *name);
 	//若成功返回0; 若出错,返回-1
```

* putenv中str是"name=value"形式的字符串,将其放入环境表中。如果name已经存在,
  则先删除其原来的定义。
* setenv将name设置为value。如果name存在则rewrite不为0,覆盖原来的值;若rewrite
  为0,则不设置。
* unsetenv 删除环境变量name。


###函数setjmp和longjmp
在C中,goto语句是不能跨越函数的,想要跨函数需要setjmp和longump两个函数。
```c
 #include <setjmp.h>

 int setjmp(jmp_buf env);
	//若直接调用,返回0; 若从longjmp返回,则为非0
 
 void longjmp(jmp_buf env, int val);
```
* env 存放着在调用longjmp时能用来恢复栈的所有信息。因为需要在别的函数中
 引用env变量,所以一般将env变量定义为全局变量。

在需要返回的位置调用setjmp,调用longjmp可以返回到调用setjmp的位置。

例子：
```c
 #include <setjmp.h>

 jmp_buf env;

 int
 main(int argc, char **argv){
	int f = setjmp(env);
	if (f != 0) { //如果是直接调用,该函数肯定返回的是0
		printf("被longjmp调用,返回值%d\n",f);	
	}

	cmd();
 }

 void
 cmd(){
	//直接调用该函数,返回到setjmp处
	longjmp(env,1);
 }

```

在调用longjmp函数后,变量是回滚到调用setjmp之前,还是保持当前值,变量不同则行为也不同:
```c
 jmp_buf env;
 int globval;	//全局变量,保持当前值,不回滚

 int
 main(){
     int 		autoval;  //自动变量,不确定
     register int 	regival;  //寄存器变量,不确定
     volatile int	volaval;  //易失变量,保持当前值,不回滚
     static int		statval;  //静态变量,保持当前值,不回滚
 }

```


###进程标识
```c
 #include <unistd.h>

 //获取进程id
 pid_t getpid(void);

 //获取进程父id
 pid_t getppid(void);

 //获取进程实际用户id
 uid_t getuid(void);

 //获取进程的有效用户id
 uid_t geteuid(void);

 //获取进程实际组id
 gid_t getgid(void);

 //获取进程的有效组id
 gid_t getegid(void);
```


###函数fork
fork函数用来创建子进程,该函数调用一次返回两次。
子进程的返回值是0,父进程的返回值是新建子进程的id。

```c
 #include <unistd.h>

 pid_t fork(void);
 //子进程返回0, 父进程返回子进程id；出错返回-1
```

当fork之后是父进程先执行还是子进程先执行是不确定的。
fork的一个特性是父进程的所有打开的文件描述符都被复制到子进程中。
并且对于复制的文件描述符,父进程和子进程共享同一个文件偏移量。
假定所有的描述符是在fork之前打开的,父进程和子进程写同一描述符指向的文件,
但又没有任何形式的同步,那么他们的输出将是混乱的。
子进程不继承父进程设置的文件锁。


###函数wait和waitpid
这两个函数用来等待子进程终止。
如果所有的子进程都还在运行,则阻塞(waitpid方法有一个选项可以设置为非阻塞)
如果一个子进程已经终止,正等待父进程获取其终止状态,则会取得该子进程的终止
状态立即返回。
如果没有任何子进程,则出错返回。

```c
 #include <sys/wait.h>

 pid_t wait(int *statloc);

 pid_t waitpid(pid_t pid, int *statloc, int options);
 //若成功,返回进程id; 若出错,返回0或-1
```
 * statloc 如果该参数不为空,则子进程终止后的状态存放在该参数中

使用<sys/wait.h>中的各个宏可以查看终止状态原因:
* WIFEXITED(statloc) 若子进程为正常终止,则为真。
  这时使用WEXITSTATUS(statloc)可以获取子进程退出时的返回码。
* WIFSIGNALED(statloc) 若子进程异常终止,则为真。
  这时使用WTERMSIG(statloc)可以获取子进程终止的信号值。
* WIFSTOPPED(statloc)
* WIFCONTINUED(statloc)

对于waitpid方法参数pid和options有如下行为:
* pid == -1  等待任一子进程终止。此时和wait等效。
* pid > 0 等待pid这个子进程终止。
* pid ==0 等待一个子进程终止,等待的子进程的组id,必学和调用该方法的进程的组id一致。
* pid < -1 

options:
* WCONTNUED
* WNOHANG    设置为不阻塞等待
* WUNTRACED 


###函数waitid
有Single UNIX Specification定义,类似waitpid函数
```c
 #include <sys/wait.h>
 
 int waitid(idtype_t idtype, id_t id, siginfo_t *infop, int options);
 //返回0,出错返回-1
```


###函数wait3和wait4


###函数exec
当进程调用一种exec函数时,该进程执行的程序完全替换为新程序,新程序从其main函数开始执行。
exec函数并不创建新进程,当前进程id不会改变。该函数只是用磁盘上的一个新程序替换当前进程
的正文段、数据段、堆段和栈段。

```c
#include <unistd.h>
int execl(const char *pathname, const char *arg0, ...);
int execv(const char *pathname, char *const argv[]);
int execle(const char *pathname, const char *arg0, ...);
int execve(const char *pathname, char *const argv[], char *const envp[]);
int execlp(const char *filename, const char *arg0,...);
int execvp(const char *filename, char *const argv[]);
int fexecve(int fd, char *const argv[], char *const envp[]);
//若出错，返回-1；若成功,不返回
```


###解析器文件
语法:
```shell
 #! pathname [potional-argument]
```

常见的解析器文件以"#!"行开始:
```shell
 #! /bin/sh
 #! /bin/awk -f
 #! /bin/echo hello world
```

* 解析器文件是一个文本文件,它以#!开头
* 解析器是解析器文件中第一行中的pathname

例子1:
如果有一个解析器文件ncat内容如下
```shell
 #! /bin/cat /my/path/aa.txt
```

执行
```shell
 $ ncat
```
则会打印出 /my/path/aa.txt 中的内容。


例子2:
如果有一个解析器文件nawk内容如下
```shell
 #! /bin/awk -f
 BEGIN {
    for (i = 0; i < ARGC; i++)
	printf "ARGV[%d] = %s\n", i, ARGV[i]
    exit
 }
```

执行
```shell
 $ nawk
```
则nawk文件中从第二行开始的所有内容都被视为awk的程序。

如果要用awk执行,其命令参数如下
```shell
 $ /bin/awk -f  /my/path/nawk
```
因为符号"#"正好是awk的注释符,所以*$ nawk* 和 *$ /bin/awk -f /my/path/nawk*等价。


###htonl
 将无符号长整形的本地(host)字节序转换为网络字节序(大端法)
 ```c
	 unsigned long int htonl(unsigned long int host);
 
 ```

###htons
 将无符号短整形的本地(host)字节序转换为网络字节序(大端法)
 ```c
	unsigned short int htons(unsigned short int host);
 ```

###ntohl
 将无符号长整形网络字节序转成本地(host)字节序
 ```c
	unsigned long int ntohl(unsigned long int net);
 ```
 
###ntohs
 将无符号短整形网络字节序转成本地(host)字节序
 ```c
	unsigned short int ntohs(unsigned short int net);
 ```

###套接字结构体
```c
	//网络地址(Internet address)一个无符号整形
	struct in_addr {
		in_addr_t s_addr;
	};

	//套接字地址(socket address),因特网样式的(internet style)
	//写代码时我们使用这个结构体赋值,然后强转成内核地址(struct sockaddr)
	struct sockaddr_in {
		__uint8_t	sin_len;
		sa_family_t	sin_family;
		in_port_t	sin_port;
		struct in_addr	sin_addr;    //网络地址
		char		sin_zero[8];
	}

        //内核使用的地址套接字地址,内核函数都接收这种地址	
	struct sockaddr {
		__uint8_t	sa_len; //用一个字节表示的长度
		sa_family_t	sa_family;
		char		sa_data[14];		
	}
```

###inet_aton
 将字符形的ip地址转换成整形ip地址(网络IP地址如,struct in_addr)。
 成功返回1, 出错返回0
 ```c
	int inet_aton(const char *strip, struct in_addr *intip);
 ```

###inet_ntoa
 将一个网络ip地址(struct in_addr)转换成字符串形式的ip
 ```c
	char *inet_ntoa(struct in_addr in);
 ```

###socket <netinet/in.h>
 创建一个套接字描述符
* domain: AF_INET(因特网)
* type:	  SOCK_STREAM(因特网的一个端点)
* protocal: 0 
* 返回套接字描述符,出错则返回-1	 
```c
	int socket(int domain, int type, int protocol);
```

###connect
* 用套接字描述符和一个网络地址建立连接(类似bind方法)。
* sockfd: 一个套接字描述符
* sock_addr: 内核套接字地址(struct sockaddr)
* addrlen: 网络套接字对象长度 sizeof(sockaddr_in) 
* 成功返回0,出错返回-1
```c
	int connect(int sockfd, struct sockaddr *sock_addr, int addrlen);

	例子:
	int socketfd = socket(AF_INET, SOCK_STREAM, 0);

   	struct sockaddr_in sock_addr_in;
	sock_addr_in.sin_family=AF_INET;
	sock_addr_in.sin_port=htons(80);
	inet_aton("192.168.134.89", &sock_addr_in.sin_addr);	
	
	int result = connect(socketfd, (struct sockaddr *)&sock_addr_in, sizeof(sock_addr_in));	
```

###setsockopt
 为套接字设置一些参数,比如tcp的缓冲区大小

* sockfd: 套接字描述符
* level: 套接字存在的层次
	* SOL_SOCKET: 通用套接字,该层支持的选项(optname)如下
		* SO_BROADCAST: 延迟关闭连接
		* SO_DEBUG: 允许调试
		* SO_DONTROUTE: 不查找路由
		* SO_ERROR: 获得套接字错误信息
		* SO_KEEPALIVE: 保持连接
		* SO_LINGER:是否延迟关闭连接(true|false),如果设置为0,则当调用close方法时直接发送RST报文来关闭连接
		* SO_OOBINLINE:
		* SO_RCVBUF: 接收缓冲区大小
		* SO_SNDBUF: 发送缓冲区大小
		* SO_RCVLOWAT: 接收缓冲区下限
		* SO_SNDLOWAT: 发送缓冲区下限
		* SO_RCVTIMEO: 接收超时
		* SO_SNDTIMEO: 发送超时
		* SO_REUSEADDR: 是否允许socket绑定处在TIME_WAIT状态的连接(true|false)
		* SO_REUSEPORT: 是否允许多个socket绑定到相同的连接(ip和port相同)上。如果
				第一个绑定某个地址和端口的socket没有设置SO_REUSEPORT,那么
				其他的socket无论有没有设置SO_REUSEPORT都无法绑定到该地址和
				端口直到第一个socket释放了绑定。
		* SO_TYPE: get socket type		
	* IPPROTO_TCP: 该层支持的选项如下
		* TCP_MAXSEG: TCP最大数据段的大小
		* TCP_NODELAY: 不使用Nagle算法
	* IPPROTO_IP 
	* IPPROTO_IPV6

* optname: 需设置的选项,该值范围有level决定
* optval: 存放选选项值的缓冲区
* optlen: optval缓冲区长度
```c
	int setsockopt(int sockfd, int level, int optname, 
			const void *optval, socklen_t optlen);	

```

###bind
 将套接字描述符和网络地址绑定上
* sockfd: 套接字描述符
* sock_addr: 内核网络地址
* addrlen: 网络地址长度
* 成功返回0, 出错返回-1
```c
	int bind(int sockfd, struct sockaddr *sock_addr, int addrlen);
	使用方式跟connect类似
``` 

###listen
 将套接字设置为监听状态
* sockfd: 套接字描述符
* backlog: 等待队列大小
* 成功返回0, 错误返回-1
```c
	int listen(int sockfd, int backlog);
``` 

###accept
 从监听的套接字描述符中,获取网络套接字描述符(包括内核地址)
* listenfd: 用于监听的套接字描述符
* *sock_addr:(可选)要回填的地址(struct sockaddr)
* *addrlen:(可选)要回填的地址长度
* 返回网络描述符,出错返回-1
```c
	int accept(int listenfd, struct sockaddr *sock_addr, int *addrlen);

	例子:
	struct sockaddr_in client_addr;
	int addrlen;
	int client_fd = accept(listenfd, (struct sockaddr*)&client_addr, &addrlen);
```

###epoll_create
 创建一个epoll描述符。
* size: 可处理的描述符大小,一个建议值；Linux2.6.8之后会忽略这个值,但必须是正数 
* 返回epoll描述符,错误返回-1
```c
    #include<sys/epoll.h>
	int epoll_create(int size);
```

###epoll_ctl
* control interface for an epoll file descriptor
* 向epoll中注册事件的函数

* epfd: epoll对象的文件描述符
* op: 操作类型
   * EPOLL_CTL_ADD: 将fd放入到epfd中,不可重复添加,重复添加会报错 
   * EPOLL_CTL_MOD: 修改fd在epfd中注册的事件
   * EPOLL_CTL_DEL: 将fd从epfd中删除
* fd: 要注册事件的文件描述符
* *event: 要注册的事件
  ```c
     typedef union epoll_data {
	void		*ptr;
	int 		fd;
	uint32_t	u32;
	uint64_t	u64;
	
     } epoll_data_t;

     struct epoll_event {
	uint32_t	events; //二进制掩码
	epoll_data_t	data;	
     }
     
     events这个二进制掩码有如下的可用类型:
     EPOLLIN: 关联的fd对于read函数可操作
     EPOLLOUT: 关联的fd对于write函数可操作
     EPOLLRDHUP: (Linux 2.6.17) 表示TCP连接的远端关闭或半关闭连接
     EPOLLPRI: 对应的连接上有紧急数据需要连接
     EPOLLERR: 连接发生错误
     EPOLLHUP: 连接被挂起
     EPOLLET: 设置为边缘触发(Edge Triggered),默认是水平触发(Level Triggered)
     EPOLLONESHOT: (Linux 2.6.2以后)只监听一次,想继续监听则需将fd在此放入到epoll中
     EPOLLWAKEUP: (Linux 3.5)
     EPOLLEXCLUSIVE: (Linux 4.5)
  ```  
* 成功返回0, 失败返回-1 

```c
    #include<sys/epoll.h>
	int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
```  


###EPOLLONESHOT的作用
  该标志在Linux 2.6.2以后才有。
  描述符打上该标记后,不管该描述符上有多少事件,以及该描述符是哪种触发模式(ET或LT),都值触发一次。
  打上该标记后,类似于描述符得到事件通知后马上从epoll对象中删除该描述符对应的时间,并且这是一个原子操作, 
  后续如果想再次收到该描述符的通知,那就必须再次调用epoll_ctl()方法将事件放到epoll中。

  该标志可以解决多进程(或线程)并发处理某个描述符时带来的竞争问题,比如有多个线程同时epoll_wait()同一个
  描述符,当有读事件反复到来的时候,很可能多个线程要同时处理同一块数据,这就产生了竞争,除非加锁,否则
  谁都不知道某块数据是不是应该它来处理。解决这个问题可以使用该标记,当通知到来后,如果不调用epoll_ctl()
  把事件重新放入到epoll中,那后续永远都收不到该描述符的事件,等数据数据处理完毕后再将其放入epoll,然后其他
  线程就又可以公平竞争了。

  打上该标记后epoll_wait()不会产生惊群   

  打上该标记后ET和LT模式好像就没有区别了?


###epoll_wait
 等待I/O事件的到来
* epfd: epoll对象描述符
* *events: 用于填充的就绪事件集合(数组) 
* maxevents: 设定最多可以返回多少就绪事件
* timeout: 超时时间,0会立即返回,-1表示indefinitely 
* 成功则返回就绪描述符的个数,如果返回0则表示超时了,错误则返回-1
```c
    #include<sys/epoll.h>
	int epoll_wait(int epfd, struct epoll_event *events, 
			int maxevents, int timeout);

	例子:
	int epfd = 5;
	int maxevents = 1024;
	int timeout = 1000;
	struct epoll_event events[1024];
	
	int result = epoll_wait(epfd, events, maxevents, timeout);
```


###epoll中epoll_wait()方法返回的条件 
 只要内部的一个队列rdllist不为空就可以返回,有两种方式可以让该队列不为空 

 第一种:文件描述符状态改变(可以认为由网卡驱动触发)
 第二种:文件描述符的事件标志位被打标了(可以认为在调用epoll_wait()方法时触发)

 --->如下几种状况发生时会触发文件描述符的改变 
 1.读缓冲区由空变为不空时 
 2.读缓冲区有新数据到达时 
 3.写缓冲区由不可写变为可写时(比如,缓冲区从满到不满)
 4.写缓冲区有数据被取走时(比如客户端从服务端的写缓冲区读走部分数据时)
 5.监听的文件描述符有新建连接时(如果好几个客户端同时到达,则只提醒一次)
 
 --->如下几种状况发生时会触发事件标志位被打标 
 1.读缓冲区中有数据可读(也就是数缓冲区不为空)
 2.写缓冲区未被写满 

 --->初始过程举例 
 1.当第一次调用epoll_wait()方法后,需要等文件描述符状态改变后,才会触发rdllist不为空,并唤醒等待进程。 
 2.epoll_wait()方法被唤醒后会先拷贝一份数据到txlist队列中,然后清空rdllist队列。 
 3.随后遍历txlist队列,在获取每个描述符最新的events标志位后,把就绪数据拷贝到用户空间(供外循环使用)
 4.遍历的过程中,如果描述符不是ET模式则将其重新加入rdllist队列,这样下一个IO循环会立即返回 
    (?外循环会收到没有任何就绪事件的fd吗? 或者说在第3步的时候会检查出,然后不把数据拷贝到用户空间?)   
 5.遍历的过程中,如果描述符是ET模式就不会重新加入到rdllist队列。 

 
###epoll操作文件描述符的两种模式 
 LT模式:也叫水平触发,文件描述符状态改变和事件标志位被打标都会触发。 

 ET模式:也叫边沿触发,只有文件描述符状态改变才会触发。 


###ET模式的accept()方法(监听描述符需要是非阻塞,否则该方法会被阻塞)
 为了避免漏掉同时到达的客户端连接,需要一直循环处理完就绪队列中所有连接后再退出。 
 (返回-1并且errno为EAGAIN表示处理完毕)



###sigemptyset
* 将set中的信号集合初始化并清空
* *set: 信号集合
* 成功返回0,错误则返回-1
```c
	#include <signal.h>
	int sigemptyset(sigset_t *set);
```


###sigprocmask
* 屏蔽或解除屏蔽当前进程的信号;屏蔽的意思就是信号过来后不会立即
  执行信号绑定的方法,而是阻塞在内核。

* 另外,如果在信号函数正在执行时,进程又收到了同样的信号,那么在该函数
  执行完之前,只会保留一个后续信号,所以这里把进程接收信号的装置看成
  一个布尔变量会好理解一点;当信号来了,变量为true,然后将变量设置为fasle并开始
  执行信号绑定的函数,后续不管来多少这个信号,都是把变量置为true,然后内核会
  判断当前是否有正在执行的信号函数,如果没有则执行绑定的函数。
* how:
   * SIG_BLOCK: 为当前进程追加屏蔽信号
   * SIG_UNBLOCK: 删除当前进程set集合中的屏蔽信号,保存oldset中的信号
   * SIG_SETMASK: 重置当前进程中的屏蔽信号为set中的信号 

```c
	#include <signal.h>
	int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);


	#例子:
	#include <stdio.h>
	#include <signal.h>
	#include <unistd.h>
	#include <string.h>

	void func(int sig){
		printf("--\n");
	}

	int main(int argc, char** argv){
		sigset_t set;
		sigset_t set_empty;
	
		// 清空这两个信号集合
		sigemptyset(&set);
		sigemptyset(&set_empty);

		//添加一个键盘终端信号(ctrl + c)
		sigaddset(&set, SIGINT);

		//为当前进程的绑定SIGINT信号,并为这个信号绑定一个方法
		struct sigaction sa;
		sa.sa_handler = func;
		sigemptyset(&sa.sa_mask);
		sigaction(SIGINT, &sa, NULL);

		//首先为进程屏蔽绑定SIGINT信号
		sigprocmask(SIG_BLOCK, &set, NULL);


		while(1){
			// 不停的向标准输出中输出*号
			write(1, "*", strlen("*"));

			// 临时放过set_empty集合之外的信号,等执行完一个信号后
			// 再恢复到原来的信号屏蔽值,这样每接收一个SIGINT信号
			// 就会打印一个 "--\n",如果没有任何信号,则会一直阻塞 
	#if 1
			sigsuspend(&set_empty);
	#else
			// 撤销对该进程的set中的信号屏蔽
			sigprocmask(SIG_UNBLOCK, &set, NULL);
			
			// 每隔一秒钟打印一个*号
			// 此时,每发送一次SIGINT信号就会发送一个"--n"并提前
			// 打断sleep(会提前醒来)
			sleep(1);
	#endif
		}	

	}

```


###sigsuspend
* 将信号屏蔽,临时设置成set中的信号,在接收到一个信号(set之外的)之前
  当前进程会被阻塞;
  当接收到一个信号并处理完信号函数后,信号屏蔽会恢复到原来的值。
* 信号函数在用户态执行

```c
	#include <signal.h>
	int sigsuspend(const sigset_t *set);
```


###sigaction
* 设置信号sig要处理的函数,和sigal函数类似
* sig: 信号
* *restrict: 信号函数放在该结构体中,如sigaction.sa_handler,
	sa_handler是一个宏#define sa_handler __sigaction_u.__sa_handler
* 成功返回0,错误返回-1
* 信号函数在用户态执行

```c
	#include <signal.h>
	int sigaction(int sig, const struct sigaction *restrict act
			struct sigaction *restrict oact);

```

###信号SIGALRM
 liunx中的信号,函数alarm和setitimer会产生SIGALRM信号


###setitimer
 设置定时器,会覆盖上一次设置的同一种类型定时器
* which: 定时器类型
	* ITIMER_REAL: 记录一个真实的时间,触发后发送SIGALRM信号
	* ITIMER_VIRTUAL: 记录进程运行的时间,不包括系统调用,触发后发SIGVTALRM信号
	* ITIMER_PROF: 记录进程使用的cpu时间,包括系统调用,触发后发SIGPROF信号
* *new_value: 要设置的时间
* *old_value: 如果该值不为空,则返回上一次的定时器设置
* 成功返回0,错误返回-1

```c
	struct timeval{
		time_t		tv_sec; //秒
		suseconds	tv_usec; //微秒
	}

	struct itimerval{
		struct timeval	it_interval; //间隔时间
		struct timeval	it_value; //定时器启动时间
	}

	#include <sys/time.h>
	int setitimer(int which, const struct itimerval *new_value,
			struct itimerval *old_value);
```


###getrlimit/setrlimit
 获取或设置资源限制
* resource: 要获取或设置的资源
	* RLIMIT_NOFILE: 当前进程可代开的最大文件描述符
	* RLIMIT_AS: 允许进程使用的最大虚拟空间
* *rlim: 描述资源闲置的软硬结构体

```c
	struct rlimit{
		rlim_t	rlim_cur; //软限制,内核限制的资源
		rlim_t	rlim_max; //硬限制,软限制不可大于硬限制
	};

	#include<sys/resource.h>
	int getrlimit(int resource, struct rlimit *rlim);
	int setrlimit(int resource, const struct rlimit *rlim);
```


###内存映射函数mmap
* 将文件描述符映射进用户空间,按页大小来映射,多余空间补零。
  比如需要映射1个字节的内存,但是系统页大小是4KB,最终会映射
  到用户空间4KB的内存,除了第一个字节,其它都是0。
* *addr: 映射进用户空间的开始地址,如果不指定则有内核决定 
* length: 需要映射的内存字节个数 
* prot: 映射内存的保护模式,有如下四中方式:
	* PROT_EXEC: 映射的页内存可以被执行
	* PROT_READ: 映射的内存可以读
	* PROT_WRITE: 可以被写
	* PROT_NONE: 不能被访问
* flags: 指定映射模式:
	* MAP_PRIVATE: 创建一个隐私的copy-on-write映射,对该内存的更新,其它进程不可见 
	* MAP_SHARED: 共享映射,更新操作对其它进程可见
	* MAP_ANON: 同MAP_ANONYMOUS;(deprecated)
	* MAP_ANONYMOUS: 匿名映射,不会指向任何文件,
		设置该标志位后fd最好为-1,offset为0。
* fd: 要映射的文件描述符,当flags为MAP_ANONYMOUS时设置为-1
* offset: 要映射的文件描述符的偏移位,当flags为MAP_ANONYMOUS时设置为0 

* 成功则返回一个分配好的指针,失败则返回MAP_FAILED((void *)-1)*

```c
	#include <sys/mman.h>
	void *mmap(void *addr, size_t length, int prot, int flags,
			int fd, off_t offset);	
```


###setsid
* 每打开一个终端或一个登入都是一个会话,每个会话都有一个唯一的前台进程组和多个后台进程组,
  只有前台进程组才会有会话终端。
* 子进程调用setsid方法会使子进程晋升为进程组长,如果进程已经是进程组长则返回错误。
  这样子进程就和原来的进程完全脱离了关系,并且子进程也不会被分配控制终端。
* 如果想重新打开控制终端可以调用open("/dev/ttyn")方法。
  如果不想该进程重新打开控制终端,则可以在fork()一次,这样第一个子进程退出,新fork()出的子进程
  就变成了当前进程,并且因为新fork出的进程不是进程组长,所以就无法重新再打开控制终端。


###sigaddset
* 添加一个信号signo到信号集合set中
* 成功返回0,错误返回-1并且设置errno

```c
	#include <signal.h>
	int sigaddset(sigset_t *set, int signo);
``` 


###setgid
* 设置进程组ID


###ioctl
(下面这段描述来自网络)
* 虽然在文件操作结构体"struct file_operations"中有很多对应的设备
  函数,但是有些命令是是在找不到对应的操作函数。比如CD-ROM的驱动,
  想要一个弹出光驱的操作,这种操作并不是所有的字符设备都需要,所以
  文件操作结构体也不会有对应的函数操作。出于这种原因,ioctl就有了
  它的用处:一些没办法归类的函数,就统一放在ioctl这个函数操作中,通过
  指定的命令来实现对应的操作。

* 对于操作套接字和普通文件描述符来说,有些功能和其它函数是有重合的

```c
	#include <unistd.h>
	int ioctl(int fd, int cmd, ... /* void *arg */);
```

其中,第三个参数总是一个指针,指针类型依赖cmd。
* cmd: 传入的命令参数
  * SIOCATMARK
  * SIOCSPGRP: 获取进程ID或进程组ID; 等同fcntl的F_GETOWN命令。 
  * SIOCGPGRP: 设置进程ID或进程组ID; 等同fcntl的F_SETOWN命令。 

  * FIONBIO: 设置fd的阻塞和非阻塞标志; 等同fcntl的F_SETFL命令的O_NONBLOCK标记。 
  * FIOASYNC: 等同fcntl的F_SETFL命令的O_ASYNC标记。
  * FIONREAD
  * FIOSETOWN
  * FIOGETOWN


###strncmp
* 比较两个字符大小,最多比较n各字符
* *s1: 原字符串
* *s2: 要比较的字符串
* 返回0表示相等; s1>s2则返回大于0的值; s1<s2则返回小于0的值 

```c
	#include <string.h>
	int strncmp(const char *s1, const char *s2, size_t n);
```


###gethostname
* 获取主机名字
* *name: 主机名字会填充到这里
* len: *name指向的数组长度
* 如果实际主机名字的长度大于len,则名字会被截断
```c
	#include <unistd.h>
	int gethostname(char *name, size_t len);
```


###socketpair
* 创建一对没有绑定的socket描述符
* domain: 指定使用哪种通信域来创建这对socket套接字
    * SOCK_STREAM: 提供有序、可靠、双向的字节流模型(类似TCP)
    * SOCK_DGRAM: 提供无连接状态的数据报(类似UDP)
    * SOCK_SEQPACKET:
* type: 指定创建哪种socket类型	
* protocol: 如果是非0,则需要指定一个address family支持的协议;0则系统使用该默认实现。 
* socket_vector: 用来接收被创建的socket套接字(注意是未被绑定的)
* 成功返回0,失败返回-1

```c
	#include <sys/socket.h>
	int socketpair(int domain, int type, int protocal, int socket_vector[2])

	// 用于主子进程通信的例子:
	#include <sys/socket.h>
	#include <stdio.h>
	#include <errno.h>
	#include <stdlib.h>
	#include <unistd.h>
	#include <string.h>
	#include <unistd.h>	
	
	int main(int argc, char **argv){
		int domain = AF_UNIX;
		int type = SOCK_STREAM;
		int protocol = 0;
		int sockets[2];

		if(socketpair(domain, type, protocol, sockets[0] == -1)){
			printf("创建套接字对错误[%d]\n", errno);
			return -1;	
		}


		// fork子进程
		int pid = fork();
		
		if(pid == -1){
			printf("fork子进程时错误\n");
			return -1;
		}

		
		// 子进程
		if(pid == 0){
			char buf[1];

			while(1){
				// 一直读,每次读一个字符
				ssize_t n = read(sockets[1], buf, sizeof(buf));
				if(n < 1){
					sleep(1);
				}
				
				// 读完一个字符就输出到标准输出
				write(STDOUT_FILENO, buf, sizeof(buf));
				write(STDOUT_FILENO, "\n", sizeof("\n"));
			}
		}

		
		// 主进程
		int i = 0;
		char *str = "hello";
		while(1){
			write(sockets[0], str, strlen(str));
			sleep(1);
		}	
	}
```


###sendmsg、recvmsg
* 所有的read/readv/recv/recvfrom都可以替换成revcmsg。各种输出函数也可以替换成sendmsg
* 该函数可以接收或输出一个结构体(是不是有点像java中的序列化)
* 成功则为读入或写出的数据,出错则为-1

```c
	#include <sys/socket.h>
	ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags);
	ssize_t sendmsg(int sockfd, struct msghdr *msg, int flags);

	
	#通过msghdr结构体来发送数据(消息)
	struct msghdr {
		/*
		 * name指向一个套接字地址结构体,用来存放对端的结构地址(比如套接字地址)。
		 * 如果无需指明则name应该设置为空.
		 * 
		 * 对于recvmsg函数,name是需要回填的结果参数。
		 * 对于recvmsg函数,namelen指定name的内存大小,同时也是内核要回填的值,是
		 * 一个值结果参数  
		 */
		void		*msg_name;
		socklent_t	msg_namelen;

		
		/*
		 * 这两个指定输入输出的缓冲区数组(注意msg_iov是个数组)
		 * iovlen是数组的个数,比如: iov[3]那么iovlen就应该等于3
		 * 数组元素是 struct iovec
		 *   struct iovec {
		 *	void 	*base;   //缓冲区开始地址
		 *	size_t	iov_len; //缓冲区长度 
		 *   }
		 *
		 * iovlen和namelen一样是个值结果参数。
		 */	
		struct iovec	*msg_iov;
		int 		msg_iovlen;
	

		/*
		 * 指定可选的辅助数据(控制信息)
		 * 
		 * controllen和namelen一样是个值结果参数 
		 *
		 * msg_control指向一个cmsghr结构体,用来传递或接收辅助信息
		 * struct cmsghdr {
		 *	// 数据和结构体的总长度
		 *	socklent_t	cmsg_len;
		 *	// 协议级别:IPPROTO_IP|IPPROTO_IPV6|SOL_SOCKET
		 *	int 		cmsg_level;
		 *	// 协议类型:SCM_RIGHTS(辅助数据是一个文件指针)
		 *	int 		cmsg_type;
		 *	/* char cmsdata[] 数据  */
		 * }
		 */
		void		*msg_control;
		socklen_t	msg_controllen;
	
		// 只有recvmsg使用该成员,该函数被调用时,入参flags会被复制到
		// 该成员变量中,也是一个值结果参数
		int 		msg_flags;
	}


	
	// 一个使用这两个方法的例子
	#include <sys/socket.h>
	#include <stdio.h>
	#include <errno.h>
	#include <stdlib.h>
	#include <unistd.h>
	#include <string.h>
	#include <unistd.h>


	int main(int argc, char **argv){

	        // UNIX套接字
        	int domain = AF_UNIX;
       		int type = SOCK_STREAM;
        	int protocol = 0;
        	int sockets[2];

        	if(socketpair(domain, type, protocol, sockets) == -1){

                	printf("创建套接字对错误[%d]\n", errno);
                	exit(-1);
        	}


        	// fork子进程
        	int pid = fork();

		if(pid == -1){
                	printf("fork子进程是错误\n");
                	return -1;
        	}


        	typedef struct {
                	int      age;
       		} person_t;

        	// 子进程
        	if(pid == 0){
                	person_t sub;

                	struct iovec iov[1];
                	struct msghdr   msg;

                	while(1){

                        	// 用于接收数据的缓冲区开始地址
                        	iov[0].iov_base = (char *)&sub;
                        	// 缓冲区长度
                        	iov[0].iov_len = sizeof(person_t);

                        	msg.msg_name = NULL;
				msg.msg_namelen = 0;
                        	msg.msg_control = NULL;
                        	msg.msg_controllen = 0;

                        	// 设置接收数据的缓冲区数组
                        	msg.msg_iov = iov;
                        	msg.msg_iovlen = 1;


                        	int n = recvmsg(sockets[1], &msg, 0);
                        	if(n == -1){ //出错了
                                	sleep(1);
                                	write(STDOUT_FILENO, "error", sizeof("error"));
                        	}

                        	// 输出结果
                       		printf("接收数据: %d个字节, 值是%d\n",n, sub.age);
                	}
        	}

		
		// 主进程
        	while(1){
                	person_t main;
                	main.age = 23;

                	struct iovec iov[1];
                	struct msghdr msg;

                	iov[0].iov_base = (char *)&main;
                	iov[0].iov_len = sizeof(main);

                	msg.msg_name = NULL;
                	msg.msg_namelen = 0;
                	msg.msg_control = NULL;
			msg.msg_controllen = 0;
                	msg.msg_iov = iov;
               		msg.msg_iovlen = 1;

                	printf("\n>>发送数据%zu\n", iov[0].iov_len);
                	sendmsg(sockets[0], &msg, 0);
                	// 每隔一秒发送一个person_t结构体
                	sleep(1);
        	}
	}
       		
```



###writev
* 把iov数组中的数据写到文件描述符fd中,其中iov数组中有iovcnt个struct iovec
* fd: 文件描述符
* iov: 一个数组,里面存放要写出的数据
* iovcnt: iov数组中元素的个数

```c
	#include <sys.uio.h>
	
	struct iovec {
		void *iov_base;	/* Starting address  开始地址*/
		size_t iov_len; /* Number of bytes to transfer 数据长度*/
	}

	ssize_t writev(int fd, const struct iovec *iov, int iovcnt);

```


###数据传输过程中的上下文切换和数据拷贝
要把一个文件中的数据发送到网络上,传统的方式我们需要如下两个方法
	read(int fd, void *buf, size_t nbytes);
	write(int fd, const void *buf, size_t nbytes);
这两个方法都是系统调用,我们看看这两个方法分别走做了什么

read方法数据流向
	文件 --> kernel buffer --> user buffer
这期间经历了两次上下文切换和两次数据拷贝
两次上下文切换分别是
	调用read方法,进程从用户态切入内核态
	read方法返回,进程从内核态切入用户态
两次数据拷贝分别是
	DMA把磁盘中的数据拷贝到kernel buffer中,此次拷贝并没有使用cpu
	CPU将kernel buffer中的数据拷贝到user buffer中
	
write方法的数据流向
	user buffer --> kernel socket buffer --> 协议引擎(协议栈)
这期间同样经历了两次上下文切换和两次数据拷贝
两次上下文切换分别是
	调用write方,进程从用户态进入内核态
	write方法将数据写入到kernel socket buffer后返回
两次数据拷贝分别是
	CPU将user buffer中的数据拷贝到kernel socket buffer,然后方法返回
	DMA将kernel socket buffer中的数据拷贝到协议引擎中

从上面可以看到,一个简单的数据传输居然经历了四次上下文切换和四次数据拷贝,
我们可以通过mmap的方式来替代read对这个过程做一个简单的优化

mmap方法的数据流向
	文件 --> kernel buffer(shared by user buffer)
这期间经历了两次上下文切换,但是且只发生了一次数据拷贝
两次上下文切换分别是
	调用mmap方法,进程有用户态进入内核态
	mmap方法将数据映射到user buffer后返回
一次数据拷贝是
	DMA模块将数据从磁盘拷贝到内核,这次并没有将数据拷贝到用户,只是做了一个映射

从上面的例子可以看到,如果read换成mmap则整个过程会少一次cpu拷贝,但是这样会增加
程序的复杂性,比如当此时进行write调用时另一个进程对映射的文件进行截断,那么write会被
SIGBUS信号中断,我们需要根据实际情况来出来这个信号,以免系统崩溃。


###sendfile(零拷贝)
* out_fd: 数据会发送到该描述符中;linux2.6.33之前必须是一个socket fd 
* in_fd: 必须是一个支持mmap方法的fd,不能是socket fd 
* offset: in_fd中的偏移量,如果是NULL则从in_fd开始位置传递数据,并且更新in_fd描述符
	的偏移量(offset);
	如果该变量不是NULL,那么sendfile会从in_fd的offset位置开始读取数据,当sendfile
	返回后,变量offset会被设置成in_fd文件被读走数据的最后位置,但是in_fd文件本身
	的offset不会被更改
* count: 要拷贝的字节个数 
* 正确则返回实际传输的数据,失败则返回-1并设置errno
```c
	#include <sys/sendfile.h>
	ssize_t sendfile(int out_fd, int in_fd, off_t *offset, size_t count);
```

该方法的数据流向如下:
	文件 --> kernel buffer --> kernel socket buffer --> 协议栈(协议引擎)
发生了两次数据拷贝
	DMA模块把磁盘数据拷贝到kernel bufer
	DMA模块把kernel socket buffer拷贝到协议栈
也就是说,从kernel buffer到kernel socket buffer并没有发生拷贝动作,只是sendfile方法
把kernel buffer中数据的信息给到了kernel socket buffer

发生了两次上下文切换
	用户调用sendfile方法,由用户态进入内核态
	sendfiel方法将数据信息给到kernel socket buffer后返回,从内核态进入用户态

可以看到,sendfile方法比使用read/write方法,上下文切换和数据拷贝都减少了一倍;
并且从操作系统内核的角度来看的话,彻底消除了CPU拷贝,所以linux中管他叫零拷贝


###虚拟地址和物理地址
 * 虚拟地址: 程序中用到的地址,比如使用&符号取到的地址
 * 物理地址: 内存的实际地址
 * 内核会对虚拟地址空间分页(page),对物理地址空间分页帧(page frame),一般page大小
   为4k,并且页和页帧大小一致,但是页的个数不一定和页帧的个数一致
 * 虚拟地址组成: 页号 + 页偏移量,其中页偏移量是在页中的位置
 
 举一个虚拟页个数大于物理页帧个数的例子:
 虚拟页有两个(a,b),物理页帧只有一个(A),所以a,b肯定都会映射到A上
 比如刚开始读取虚拟页a,这时候肯定区页帧A中读取数,假设随后要读页b上的数据,内核会
 先判断页a和现在的页帧A他们的号码是不是一样,如果不一样则触发缺页功能,然后内核把
 此时a对应的A中的数据放入到磁盘,然后再把b映射到A,此时A就和虚拟地址b对应上了。 
 

###在分配内存的时候为什么要数据对齐
 cpu在操作内存的时候并不是按照单字节来操作的,为拉提高读取效率,在cpu的眼里内存是
 一块一块的,这个块的大小可能是2、4、8、16等字节大小,假设cpu把内存看成是有2个字节
 的块组成的,也就是说cpu一次读内存至少是2个字节
 例一: 内存对齐的情况下从内存读2字节数据,从地址0开始
     register                  内存
      ----- 0                 ------ 0
      |   |                   |    |
      ----- 1                 ------ 1
      |   |                   |    |
      -----                   ------ 
    这种情况下内存中的数据直接就可以进入寄存器

 例二: 内存不对齐的情况下从内存读2个字节数据,从地址1开始
     register                  内存 
     ----- 0                 ------ 0
     |   |                   |    |
     ----- 1                 ------ 1
     |   |                   |    |
     -----                   ------ 2 
			     |    |
			     ------ 3
			     |    |
			     ------
    这时候需要从内存读取1、2两个地址的字节数据,但是在cpu眼里只能看到块1(编号0,1)
    块2(编号2,3),所以就需要把块1和块2的数据都读到cpu寄存器,然后在整合成一个完整
    数据,这种显然浪费空间又浪费时间,所以有些cpu干脆罢工抛异常。

 				
###内核读磁盘文件的对齐方式(directio) (?)
  像cpu读内存时分块一样,内核在处理磁盘IO时也是需要分块的,一般块大小是pagesize
  也就是说在内核的眼里,文件的大小都是按照pagesize块大小组成的,这里可以看到内存
  的角色其实跟cpu中的寄存器是一样的,磁盘则对应内存,正常情况下我们要求这两个都要
  对齐

  如果内存不对齐,则有可能从磁盘读512字节需要需要两块内存在接收  
    

###directio
  direcctio方式读数据时有几个条件需要注意
  1.接收数据的内存需要对齐,单位是块,按文件系统块大小(一般4096字节)或设备块大小(一般512字节)
    都可,ngx使用posix_memalign()来分配自定义粒度的对齐
  2.读偏移量也需要按照块大小对齐
  3.读字节个数size也需要按照块大小对齐
 
  不满足这几个条件则直接返回错误 "Invalid argument" 


###memmove
* 从src中拷贝n个字节放入到dest中,如果这两个区域的内存有重叠,该方法会先把src才被到一个
  临时内存,然后再从临时内存拷贝到dest中.如果没有重叠则同memcpy()方法. 
* 返回指向dest的指针

```c
	#include <string.h>
	
	void *memmove(void *dest, const void *src, size_t n);
```


###sysconf
* 获取当前系统限制值或特征选项,比如_SC_NPROCESSORS_ONLN获取当前可用处理器个数 

```c
	#include <unistd.h>
	
	long sysconf(int name);
```


###getsockname
* 通过socket描述符sockfd获取这个描述符的socket信息
* addr: 用来接收socket的内存空间
* addrlen: 该值一般会被初始化为addr空间的大小;方法执行成功后会被填充成实际
      socket大小,另外如果addr空间要小于实际socket的大小,那么返回的实际socket
      数据将会被截断,那么此时返回的addrlen会比实际传入的值要大,所以可以通过
      addrlen的先后值来判断socket是否被截断

```c
	#include <sys/socket.h>

	int getsockname(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

	#例子
        struct sockaddr_in addr;
	int addr_len = sizeof(addr);

	getsockname(sockfd, (struct sockaddr *)&addr, &addr_len);	
```


###linux中man后面数字的意思
* 1、Standard commands (标准命令)
* 2、System calls (系统调用)
* 3、Library functions (库函数)
* 4、Special devices (设备说明)
* 5、File formats (文件格式)
* 6、Games and toys (游戏和娱乐)
* 7、Miscellaneous (杂项)
* 8、Administrative Commands (管理员命令) 

带p字符表示是posix规范
* 0p: posix headers
* 1p: posix utilities
* 3p: posix functions


###qsort
* 排序一个数组,默认是升序
* base: 要排序数组的起始地址
* nmemb: 要排序的数组中元素的个数
* size: 单个元素的大小 
* compar: 比较函数,返回值必须是一个整数小于、等于或大于零

```c
   #include <stdlib.h>
   
   void qsort(void *base, size_t nmemb, size_t size,
				int (*compar)(const void *, const void *))
    
```


###getsockopt/setsockopt
* 获取和设置socket配置项
* sockfd: 对应的socket描述符
* level: 协议层,告诉当前方法要操作的协议层 
* optname: 要做的具体操作,不同的协议层有不同的值,文件<sys/socket.h>中
          定义了socket-level(SOL_SOCKET)层的具体操作
* optval: 用来接收或设置配置项内容的一个buffer,大多数socket级别选项
          使用int参数作为buffer,是一个结果值参数
* optlen: 用来指定和返回optval的字节个数,是一个结果值参数

* 成功返回0, 错误返回-1并且errno被指定


```c
   #include <sys/types.h>
   #include <sys/socket.h>

   int getsockopt(int sockfd, int level, int optname,
					void *optval, socklen_t *optlen)

   int setsockopt(int sockfd, int level, int optname,
					const void *optval, socklen_t optlen)
```








