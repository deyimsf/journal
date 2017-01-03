#数组
```python
>>> months = ["January","February","March","April"];

```

#截取数据元素

```python
>>> numbers = [1,2,3,4,5,6,7,8,9,10];

#取下标[2,5)之间的数据(不包括5)
>>> numbers[2,5]
[2,3,4]

#取数时加上步长
>>> numbers[0:10:2]
[1,3,5,7,9]

>>> numbers[::2]
[1,3,5,7,9]
```

#数组相加
```python
>>> [1,2,3] + [4,5]
[1,2,3,4,5]
```

#删除元素
```python
>>> del numbers[1]
>>> numbers
[1,3,4,5,6,7,8,9,10]
```

#元组
 元组和数组(列表)差不多,但是元组是不可变的序列
```python
#创建元组大部分通过圆括号括起来
>>> (1,2,3)
(1,2,3)

#空元组
>>> ()
()

#单个值的元组
>>> 33,
(33,)

#将一个列表转换为元组
>>> tuple([1,2,3])
(1,2,3)

#操作元组和列表差不多,但是元组本身没有像列表那样的方法
>>> x = (1,2,3)
>>> x[0]
(1)

#元组分片
>>> x[0:2] 
(1,2)
```

#字典
其实就是键值对映射
```python
>>> phonebook = {"Mason":"123", 'Jason':'456'}
>>> phonebook['Mason']
'123'
```  

#dict函数
通过dict创建字典
```python
>>> items = [('name','Gumby'),('age',42)]
>>> d = dict(items)
>>> d
{'age':42,'name':'Gumby'}
>>> d['age']
42
```

#字典格式化字符串
```python
>>> template = '''<html>
	<head><title>%(title)s</title></head>
	</html>'''
>>> data = {'title':'My page title'}
>>> print template % data
<html>
<head><title>My page title</title></head>
</html>
```

#跨行字符串用三个''' 或者"""
```python
   name = ''' my name 
	is zhang
	san	
    ''';
```




#字典的一些方法
*clear 清除字典中的所有项
```python
>>> d = {'a':'b'}
>>> d.clear()
>>> d
{}
```

*copy浅复制,copy后原字典和副本拥有相同的值(值不是副本)
```python
>>> x = {'name':'aa','ss':['a','b','c']}
>>> y = x.copy()
>>> y['name'] = 'bb' #副本的name键值被替换
>>> y['ss'].remove('b') #字典的值被修改(副本和原本引用同样的值)
>>> y
{'name':'bb','ss':['a','c']}
>>> x
{'name':'aa','ss':['a','c']}
```

*fromkeys方法使用给定的建建立新的字典
```python
>>> {}.fromkeys(['name','age'])
{'age':None,'name':None}

>>> dict.fromkeys(['name','age'])
{'age':None,'name':None}
```

*get获取键值
```python
>>> d = {'a':3}
>>> d.get('a')
3
```

*has_key方法检查字典是否有某个key
```python
>>> d = {}
>>> d.has_key('name')
False
```

*items字典内容按列表返回
```python
>>> d = {'a':3}
>>> d.items()
[('a',3)]

#iteritems返回一个迭代器对象
>>> it = d.iteritems()
>>> it
<dictionary-iterator object at 89283>
>>> list(it)
```

#生成器
* 任何包含yield语句的函数称为生成器
* 生成器有两部分组成:生成器的函数和生成器的迭代器
  > 生成器的函数使用def语句定义的,包括yield部分。
  > 生成器的迭代器是这个函数返回的部分。

例子:
```python
  def list(array):
	for a in array:
		yield a;

  >>> cc = list([1,2,3]);
  >>> print(cc.next())
  1
  >>> print(cc.next())
  2
  >>> print(cc.next())
  3
```

*keys、iterkeys
 keys方法将字典中的建一列表形式返回,iterkeys返回迭代器

*pop移除一个键值对
```python
>>> d = {'a':3,'b':4}
>>> d.pop('a')
3
>>> d
{'b':4}
```

*popitem 随机移除一个键值对

*setdefault 如果key有值则返回,没有则设置默认值
```python
>>> d = {'name':34}
>>> d.setdefault('name','default')
34
```

*update 利用一个字典更新另外一个字典

*values、itervalues 同keys/iterkeys


#条件语句
下面的值作为布尔表达式时都是假,其它都是真
> False None 0 "" () [] {}

Python中语句块以冒号 “:” 开始，用缩进标志整个块

```python
if num > 0:
	print 'a'
elif num < 0:
	print 'b'
else:
	print 'c'
```

#while循环
```Python
x = 1
while x <= 100:
	print x
	x += 1
```

#for循环
```Python
words = ['a','b','c']
for word in words:
	print word
```

#迭代字典
```Python
d = {'a':1, 'b':2}
for key in d:
	print key, 'corresponds to', d[key]


for key,value in d.items():
	print key, 'corresponds to', value

```

#列推导式
```Python
>>> [x*x for x in range(10)]
[0,1,4,9,16,25,36,49,64,81]
```

#定义函数
```Python
def hello(name):
	return 'Hello, '+ name + '!'
```

#关键字参数和默认值
```Python
 def hello(name,age):
	print name + ':' + age


 #定义时为函数设置默认值
 def hello(name='zhangsan',age=9):
	print name + ':' + age

 #调用函数
 hello('lisi',8)
 hello(age=8,name='lisi')
```

#收集参数,变参函数
```Python
 #一个*号代表可以接收多个位置参数
 def print_params(*params):
	print params

 #使用
 >>> print_params(1,2,3)
 (1,2,3)

 #如果要接收多个关键字参数,则需要两个*号
 def print_params(**params):
	print params

 #使用
 >>> print_params(a=1,b=2,c=3)
 {a=1,b=2,c=3}
```

#访问全局变量globals函数
```Python
  name = 'lisi'
  def say():
	print 'hello ' + globals()['name']

 
  >>> say()
  hello lisi
```

#局部变量是vals函数

#类
```Python
  __metaclass__ = type #使用新式类
  class Person:
	#使用类名+属性可以直接访问(Person.name)
	name = "default";
	
	#私有属性,使用类名+属性无法访问(Person.__age);实际上被翻译成了_Person__age,
	#所以使用Person._Person__age可以访问该属性	
	__age = 23;
	
	 #前面带有下划线的名字不会被带星号的import语句导入;(from module import *)
	_alias = "lisi";
	

	def getName(self):
		return self.name;


	def setName(self,name):
		self.name = name;


   #使用例子
   p = Person(); #这样就创建了一个对象
   p.getName(); #调用方法
   print Person.name; #直接访问类的属性
   >>> default; 
```
* 在类中的叫方法,不在类中的叫函数。
* 方法和函数区别在于,方法在被调用时会将调用者自身作为第一个参数传递给方法。

#类的定义其实就是执行代码块
```Python
   calss Person:
	print "类Person被定义...";

	def getName(self):
		return "name";

   #类代码被第一次加载时就会执行内部代码
   类Person被定义...

   #例子
   >>> p = Person();
   >>> p.getName();
   name
```

#类内部变量对其实例可见
```Python
   class Person:
	age1 = 0;
	age2 = 0;

	def init(self):
		self.age1 = self.age1 + 1; #改变某个实例的age1值,类属性值不会变
		Person.age2 = Person.age2 +1; #类属性值和实例值都会变

    #例子
    >>> p = Person();
    >>> p.init();
    >>> Person.age1
    0
    >>> Person.age2
    1
    >>> p.age1
    1
    >>> p.age2
    1

    >>> p1 = Person();
    >>> p1.init();
    >>> Person.age1;
    0
    >>> Person.age2
    2
    >>> p1.age1
    1
    >>> p2.age2
    2
```

#继承
```Python
   class Father:
	def init(self):
		self.name = "superclass";

    class Person(Father):
	def init(self)
		self.name = "subclass";
  
```

#捕获异常
```Python
   try:
	x = input("输入一个数:");
	y = input("输入第二个数:");
	print x/y;
   except ZeroDivisionError:   #捕获一个异常
	print "第二个数不能是零";
   except TypeError,e: #捕获一个异常,e代表捕获的异常信息
	print "发生异常,异常信息是:",e;
   except (NameError,UnicodeDecodeError),e:
	print "当前捕获的异常是:",e;
   except Exception,e: #该异常类是所有异常的基类
	print "其它异常:",e;
	raise e; #再次抛出这个异常
   else:
	print "没有发生异常";
   finally:
	print "不关有没有异常都会执行";
```

#特殊(魔法)方法
前后有两个下划线(__method__)的方法叫做特殊方法

#构造方法
init前后各自加上两个下划线就变成了类的构造方法
```Python
   class Person:
	__init__(self):
		print "构造方法...";
```

#构造方法重写
子类重写构造函数后,需要调用父类的构造函数.
* 调用未绑定的超类构造函数
* 使用super方法
 ```Python
	class Father:
		def __init__(self):
			print "I'm super class";

	class Person(Father):
		def __init__(self):
			Father.__init__(self); #调用父类的未绑定方法
			super(Person, self).__init__(); #或者使用super方法
 ```

#规则
在Python中只需要遵守几个规则,类就会变成一个序列(list)或者映射(map)
```Python
   class MyList:
	 def __init__(self):
		self.box = [];
  	 def __len__(self): #该方法返回集合中项目数量;如果是list则是元素个数,map则是键值对个数
		pass;
	 def __getitem__(self,index): #定义list[index]或list.get(index)方法的行为
		return self.box[index];


   class MyCounterList(list): #继承集合
	def __init__(self, *args):
		super(MyCounterList, self).__init__(*args);
		self.counter += 1;
		return super(CounterList, self).__getitem__(index);
```

#拦截特性访问(可以拦截字段和方法访问)
有四个特殊方法可以实现拦截功能
* __getattribute__(self, name) 当name被访问时自动调用(新式类中使用)
* __getattr__(self, name) 当name被访问切对象没有相应的属性是被自动调用
* __setattr__(self, name, value)  给属性赋值时调用
* __delattr__(self, name) 删除name属性时调用

```Python
  class Rectangle:
	def __init__(self):
		self.width = 0;
		self.height = 0;

	def __setattr__(self, name, value):
		if name == 'size':
			self.width, self.height = value;
		else:   #__dict__方法包含一个字典,字典里面是实例的所有属性
			self.__dict__[name] = value;
 	def __getattr__(self, name):
		if name == 'size':
			return self.width,self.height;
		else:
			raise AttributeError;
	def __getattribute__(self, name): #该方法拦截所有的属性访问,所以也会拦截__dict__
		#??如何防止递归??	
		
```

#模块
* 模块默认搜索路径
```python
   >>> import sys, pprint
   >>> pprint.pprint(sys.path)
   ['',
    '/System/.../lib/python27.zip',
    '/System/.../lib/python2.7',
    '...',
    '/Library/Python/2.7/site-packages']

   #自定义的模块可以放在上面的目录中,一般放在site-packages目录中
```
* 指定模块搜索路径
```python
   >>> import sys
   >>> sys.path.append('/My/path/python'); #为sys.path增加一个搜索路径
  
   #在操作系统中设置PYTHONPATH环境变量
   export PYTHONPATH=$PYTHONPATH:/My/path/python
```

* 模块和类一样也是一段可以被执行的程序,不同的是模块在被导入的时候回被执行
  一次,导入模块多次和导入一次效果一样。
```Python
   #hello_module.py
   def hello():
	print "Hello world!";
   
   print "load module hello_module ...";	

   #使用模块
   >>>import sys
   >>>sys.path.append('/My/path/python');
   >>>import hello_module #第一次导入
   load module hello_module ... #第一导入会打印该语句
   >>>import hello_module #第二次导入不会导入上面的语句
```

# 变量__name__ 表示当前模块名字
```python
   >>> __name__
   '__main__' #main模块
   >>> sys.__name__
   'sys'  #sys模块
```

#包,比模块更大的组织代码的方式
*如果一个目录下有__init__.py文件那么他就是一个包.
> 假设在draw目录下有__init__.py,color.py,shape.py三个文件,那么draw就是一个包

```python
   #单独加载一个包,这时没办法使用包下的模块,只用__init__.py中的内容可用
   >>> import draw

   #加载包中的一个模块
   >>> import draw.color
   >>> draw.color.hello(); #使用color模块方法时需要加上包名

   #直接导入包中的一个模块
   >>> from draw import color
   >>> color.hello();  #不需要加包名
``` 

#模块和类一起使用的一个例子
```python
   #/My/python/MyModule.py 该模块下包含类Person
   class Person:
	def __init__(self):
		print "我是构造函数";
	
   	def getName(self):
		return "My name is zhangsan";


    #使用这个类
    >>>import sys;
    >>>sys.path.append('/My/python');
    >>>import MyModule; #导入模块
    >>>p = MyModule.Person();
    >>>p.getName();
    'My name is zhangsan'

    >>>from MyModule import Person; #导入这个类   
    >>>p = Person();
    >>>p.getName();
    'My name is zhangsan'
```

#查看模块中的内容
```python
   >>>import copy
   >>>dir(copy) #会打印出copy模块中的可用的变量
   ['Error','PyStringMap','__all__',...] 

   >>>copy.__all__
  ['Error', 'copy', 'deepcopy']

  #变量__all__决定在使用 from copy import * 时可以导入的变量,如果
  #没有__all__则会导入所有不以下划线开头的全局名称  
```

#help函数,查看模块更详细的信息
```python
   >>>import copy
   >>>help(copy) #会打印出模块的更详细信息

```

#一些标准库
##sys
通过dir(sys)可以看到里面的函数或变量
```python
   >>>import sys
   >>>print sys.argv   
   [''] #因为在python客户端运行所以没有参数
```

##webbrowser 浏览器模块
```python
   #打开浏览器打开某个网址
   >>>import webbrowser
   >>>webbrowser.open('http://www.python.org')
```

##os
```python
   >>>import os
   >>>os.sep #查看路径中的分隔符
   '/'
```

##fileinput 遍历多个文件的所有行
```python
   #script.py
   import fileinput
   for line in fileinput.input():
	#line就表示文件中的一行
        num = fileinput.lineno(); #行号
        print line; #打印每行的内容
   
   #使用
   >>>python script.py aa.txt bb.txt
   ... #这里会打印aa.txt和bb.txt文件的内容
```

##集合
```python
   #创建一个set集合(无需,不可重复)
   >>>a = set([1,3,2]);

   #求集合并集
   >>>b = set([2,3,4]);
   >>>a.union(b) 或者 a | b

   #集合本身只能包含不可变(可散列)的值
   #但集合本身又是可变的,所有集合不能包含集合
   #frozent函数可以创建集合的副本,可以固化集合(将集合变为不可变)
   >>>a.add(frozenset(b));

```

#堆
```python
   #python中的堆就是一个有序队列
   >>>from heapq import *
   >>>heap = [];
   >>>heappush(heap,3); #向堆中增加一个元素
   >>>heappush(heap,2);
   >>>heap; 
   [2,3]
   >>>heappon(heap); #弹出一个最小的元素
   2
  
   #注意: 如果没用heappush建立堆,那么在使用heappush和heappop前应该使用heapify
   #利用heapify对堆进行排序
   >>>heap=[2,1,9,3]
   >>>heapify(heap)
   >>>heap
   [1,2,3,9]


```

#双端队列
```python
   #python中双端队列就是一个双向链表(猜的)
   >>>from collections import deque
   >>>q = deque(range(5)) #生成一个双端队列
   >>>q.append(5); #队列尾部增加一个元素
   >>>q.appendleft(6);#队列左侧增加一个元素
   >>>q
   deque([6,0,1,2,3,4,5])   
   >>>q.pop()
   5
   >>>q.popleft()
   6

```

#re 正则表达式模块
* compile(pattern [, flags]) 根据包含正则表达式的字符串创建模式对象
* search(pattern, string[, flags]) 在字符串中寻找模式
* match(pattern, string[, flags]) 在字符串的开始处匹配模式


#操作文件open函数
```python
   #open函数语法
   #name:文件名字
   #mode:打开文件的模式
   #     ‘r’:读 ‘w’:写 'a':追加  'b':二进制 '+':读写模式
   #buffering:缓冲区,0代表不用缓冲区;-1使用默认缓冲区;大于1表示缓冲区大小
   open(name[, mode[, buffering]])

   #例子
   >>>f = open('aa.txt','w');
   >>>f.write("hello \n");
   >>>f.write("world\n");
   >>>f.close();
   >>>f = open('aa.txt','r');
   >>>f.read(4);#读四个字节
   'hell'
```

#GUI
Python中有各种GUI平台,这里我们使用wxpython平台。
下载地址http://wxpython.org,下载并安装后看例子:
```python
   import wx
   
   app = wx.App();
   win = wx.Frame(None, title="Editor", size=(410,335));
   win.Show();

   loadBu = wx.Button(win, label="Open",pos=(225,5),size=(80,25));
   saveBu = wx.Button(win, label="Save",pos=(315,5),size=(80,25));

   app.MainLoop();
```

#字符前缀r/u
* 以r开头的字符串如 r'abd\n'表示r后面是一个非转义的原始字符串,\n在这里不代表换行就代表其分身字面意思。

* u开头表示这是一个unicode字符串

#数据库操作
操作不同的数据库,需要下载不同的python数据库模块,python为数据库编程定义了编程接口,不同数据库模块会实现这些接口。

```python
	#coding:utf8
	import sqlite3

	#连接数据库,如果库不存在则创建
	conn = sqlite3.connect("person.db");

	#用游标操作sql
	#创建一个表
	curs = conn.cursor();
	curs.execute('''
        	create table personC (
                	id   float primary key,
                	name TEXT,
                	age Float
        	)
	''');

	#插入一条数据
	query = "insert into personC values(?,?,?)";
	#用游标操作
	curs.execute(query,[1,"zhangsan",23]);

	conn.commit();
	conn.close();


	#查询数据库
	conn = sqlite3.connect("person.db");
	curs = conn.cursor();

	query = "select * from personC";
	curs.execute(query);
	
	#打印结果
	print curs.fetchall();

```

#网络套接字模块
```python
   #socket模块
   import socket;

   #urllib和urllib2模块
   from urllib import urlopen
   web_page = urlopen('http://www.python.org');
   text = webpage.read();
   print text;

   #其它
   from SocketServer import TCPServer, StreamRequestHandler

   #select和poll
   import socket,select
   a,b,c = select.select(inputs,[],[]);
   
   s = socket.socket();
   p = select.poll();
   p.register(s); 
   while True:
	events = p.poll();
        for fd, event in events:
		#do something

   #Twisted框架

```

#使用cgi写web应用
```python
   --------simple.cgi-----------
   #!/usr/bin/env python
   
   import cgi;
   form = cgi.FiledStorage();
   
   name = form.getvalue('name','world');
   
   print """
	<html>
	  <head>
		<title>web page</title>
	  </head>
	  <body>
		<h1> Hello, %s!</h1>
	  </body>
   """ % name;
```








