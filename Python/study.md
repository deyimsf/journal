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

