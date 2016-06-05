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
	off_t		st_size;
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

解析器文件是一个文本文件,它以#!开头

解析器是解析器文件中第一行中的pathname

例子1:
如果有一个解析器文件ncat内容如下
```shell
 #! /bin/cat /my/path/aa.txt
```

执行
```shell
 $ ncat
```
则会打印出 /my/path/aa.txt 中的内容

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





