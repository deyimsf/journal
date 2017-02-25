#关于getpagesize方法
 在unistd.h文件中有如下代码
 
 /* Removed in Issue 6 */
 #if !defined(_POSIX_C_SOURCE) || _POSIX_C_SOURCE < 200112L
 int	 getdtablesize(void) __POSIX_C_DEPRECATED(199506L);
 int	 getpagesize(void) __pure2 __POSIX_C_DEPRECATED(199506L);
 char	*getpass(const char *) __POSIX_C_DEPRECATED(199506L);
 #endif

 上面这段代码是方法的声明,还是方法的定义?



