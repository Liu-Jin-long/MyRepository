# 编写于2022年06月27日17时58分20秒
from pymysql import *

conn1 = connect(user='root', host='192.168.107.128', password='123', database='test', port=3306, charset='utf8')
cs1 = conn1.cursor()
# count=cs1.execute('insert into goods_cates(name) values("硬盘");')
# print(count)
# count=cs1.execute('update goods_cates set name="机械硬盘" where name="硬盘";')
# print(count)
# count=cs1.execute('delete from goods_cates where name="机械硬盘";')
# print(count)

# count = cs1.execute('select id,name from goods where id>4;')
# print(count)
# for i in range(count):
#     ret=cs1.fetchone()
#     print(ret)
# ret = cs1.fetchmany(4)
# print(ret)
# ret = cs1.fetchall()
# print(ret)

# 非安全方式
find_id = input('请输入物品id：')
# 如输入 5 or 1 (or 1=1) 永真
sql = 'select * from goods_cates where id=%s;' % find_id
print(sql)
count = cs1.execute(sql)
print(cs1.fetchall())
# 安全方式
# sql语句的参数化，可以有效防止sql注入
params=['5 or 1']
count = cs1.execute('select * from goods_cates where id=%s;',params)
print(cs1.fetchall())
# 100000数据测试有索引和无索引时间
# for i in range(100000):
#     cs1.execute('insert into test_index(title) values("index%d");' %i)


conn1.commit()
cs1.close()
conn1.close()
