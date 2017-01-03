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

print curs.fetchall();


