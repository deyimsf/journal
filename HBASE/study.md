#启动单机模式
```shell
 $ $HBASE_HOME/bin/start-hbase.sh
```

#管理界面
http://<ip>:16010/master-status

#通过shell命令行访问hbase
```shell
 $ $HBASE_HOME/bin/hbase shell
```

#列出hbase中的所有表
```shell
 hbase(main):008:0* list
```
 > TABLE 
 > mytable 
 > mytable2
 > users   
 > 3 row(s) in 0.3980 seconds

 > => ["mytable", "mytable2", "users"]

#HBase中数据的组成
 * 表(table)
 * 行(row) 在表里数据按行存储。行由行健唯一标识。行健没有数据类型,被视为byte[]
 * 列族(column family) 行里的数据按列分组,一个列族可以放很多k-v数据。
 * 列限定符(column qualifier) 可以理解为k-v中的k
 * 单元(cell) 行健、列族、列限定符一起确定一个单元,可以理解为k-v中的v。
   v也没有数据类型,被视为byte[]
 * 时间版本 单元值v有时间版本。用long型的时间戳表示。默认保存值的三个时间版本。
 * 创建表时需要指定好表的列族(后续不可修改?)
 
 举个例子:
     // 表(table),表中所有键值对中的键都是有序的
     {
	   // 行(row)，每个行中的列族数是固定的
       "row1": {
             // 列族(column family),不同列族可以有不同的列和值
             "column_family_A": {
                  // 列限定符(column qualifier),或者就叫列               
                  "name":{
                           // 列值(键是有序的版本,后面的值就是列值)
                           91823: "张三",
                           89034: "李四"
                        },

                  // 列限定符,可以任意多个 
                  "age":{
                       8709: 18,
                       7643: 25
                  }
             } 

             // 列族(column family),这个列族里面没有列，只有值
             "column_family_B":{
                 "" : {
                    9876: "A",
                    8765: "C",
                    1234: "B"
                 }
             }
        },


       // 行,需要有相同的列族,列族里面的列不做限制
       "row2": {
           // 列族
           "column_family_A": {
              // 任意列
           }

           "column_family_B": {
              // 任意列
           }
        }
     }   


#创建一个表
创建表时必须制定列族,列族可以多个(列个数后续不可修改?)

```shell
hbase(main):001:0> create 'mytable','column1','column2' 
``` 

#查看表的默认参数
```shell
hbase(main):001:0> describe 'mytable'
```

#读写数据
```shell
#write
hbase(main):001:1> put 'mytable','rowkey1','column1:name','zhangsan'

#read
hbase(main):001:1> get 'mytable','rowkey1','column1:name'

#获取某个行的所有数据
hbase(main):001:1> get 'mytable','rowkey1'

#获取某个列族的所有数据
hbase(main):001:1> get 'mytable','rowkey1','column1'

#获取表的所有数据
hbase(main):001:1> scan 'mytable'

```

#Hbase的配置文件
 * conf/hbase-env.sh
 * conf/hbase-site.xml
  > <property>
  >      <!--指定zookeeper地址--> 
  >      <name>hbase.zookeeper.quorum</name>
  >      <value>10.0.136.116</value>
  >  </property>
  >

#Hbase一些特性
 * HBase把一切数据存储为字节数据,可以在一个列族里拥有任意数量的列
 * HBase表里只有键可以建立索引
  > 键就是包括行健、列限定符、时间戳
 * 当region刷写留在内存里的行时生成了HFile,这些已经排过序,也会有序的刷写道硬盘上
 * SHA-1碰撞率低于MD5所以SHA-1在这方面优于MD5
 * 一张表有多个小的region组成,这些region有RegionServer管理
 * 一个region可以包含很多的列族
 * 一个列族在物理上用HFile存储,HFile会被切割成很多块(默认64K)放到不同的机器上

#通过REST访问HBase
```shell
 #启动REST服务
 $ hbase rest start -p 9999

 #获取JSON形式的值
 $ curl -H "Accept: application/json" http://localhsot:9999/users/rowkey/info:name
 
 #向HBase put数据
```




