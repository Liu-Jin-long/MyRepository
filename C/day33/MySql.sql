-- 两个横杠和一个空格是注释   快捷键ctrl+/
-- null不是什么都没有，而依然会占空间
-- 大小写不敏感
-- 规定表的一行（即每一条记录）在表中是唯一的实体。实体完整性通过表的主键来实现，主键值能唯一地标识表中的每一行(相同数据的两行键值也不同)。


show tables;

-- 查看创建该表时的表头属性
show create table Person;

-- 描述表结构
desc Person;

-- 添加一列，如果设定了not null，是会给这一行默认给一个空字符串
alter table Person add address varchar ( 10 ) not null;

-- 删除一列
alter table Person drop address;

alter table Person add birthday date;

alter table Person add score float(5,2) NOT NULL;

-- 增加时间戳,默认是当前时间
alter table Person add InsertTime TIMESTAMP DEFAULT CURRENT_TIMESTAMP;

-- 下面是DML
insert into Person(FirstName,Age,score) values ('liu',22,99.9);
 
-- 自动增长写为0即可 (如果不写列名)
insert into Person values(0,'xiu','er',6,'2015-11-11',5,'11-11-11');

insert into Person values(5,'xiong','er',8,'2018-01-21',34,'00-11-11');

insert into Person values(0,'xiong','da',9,'2017-12-01',56,'00-11-11');

-- 修改表中的数据
update Person set LastName='san' where personID=5;
update Person set LastName='a',score=66 where FirstName='xiu';

-- 删除表中的数据
delete from Person where personID=23;

-- 线上环境是叫生产环境，开发程序用的是测试库、开发环境

-- DQL  Data Query Language

-- *号代表查询所有列(字段)
-- DISTINCT可选，指显示结果时，是否剔除重复数据
select * from Person;
select DISTINCT(FirstName) from Person;
select personID,FirstName from Person;

-- name是关键字，可以使用``(Tab键上方)包起来
select FirstName as `name` from Person;

select score+10 from Person;

-- <>不等于
select * from Person where Age<>22;
select * from Person where Age between 8 and 14;-- 闭区间

select * from Person where Age in(8,22,19);-- 显示在in列表中的值
-- and or not与或非  尽量不使用not 性能较差
select * from Person where Age>=22 or FirstName='xiong';

select * from Person where LastName is NULL;-- 未放过数据为空
select * from Person where LastName='NULL';

select * from Person where FirstName like 'xi%';-- 模糊查询

-- 排序   ASC升序  DESC降序
select * from Person order by Age desc;
select * from Person order by FirstName;
select * from Person order by score;-- 升序AUTO_INCREMENT

-- limit的两个参数一个是起始条(类比数组 从0开始 也就是第一条)，第二个参数是获取几条
select * from Person order by score desc limit 0,3;

-- 学习外键，人员表和宠物表是一对多的关系
create table pets(petID int auto_increment not null,primary key(petID),name varchar(10),personID int default null,foreign key(personID) references Person(personID));

-- PRI主键约束 UNI唯一约束 MUL可以重复
desc pets;
desc Person;
show create table pets;
select * from pets;

-- 如果没有设置不能为NULL，外键可以为NULL，如果设置了不能为NULL，不能插入NULL
insert into pets values(0,'dog',8);
insert into pets values(0,'cat',10);
insert into pets values(4,'rabbit',8);
insert into pets values(7,'fish',null);
delete from pets where petID=7;
update pets set personID=14 where petID=7;

-- 该列表项已有NULL，不能设置该列属性为NOT NULL
alter table pets change personID personID int not null;

-- 删除外键
alter table pets drop foreign key pets_ibfk_1;
-- 添加外键
alter table pets add constraint pets_ibfk_2 foreign key(personID) references Person(personID);

-- 多对多的操作
create table TEACHER(tID int primary key,NAME varchar(20));
insert into TEACHER values(1,'Math Teacher'),(2,'Chinese Teacher'),(3,'English Teacher');

create table STUDENT(sID int primary key,NAME varchar(10));
insert into STUDENT values(1,"lily");
insert into STUDENT values(2,"lucy");
insert into STUDENT values(3,"lilei");
insert into STUDENT values(4,"lilyzhou");

create table TEACHER_STUDENT(
T_ID int,
S_ID int,
primary key(T_ID,S_ID),
constraint T_ID_FK foreign key(T_ID) references TEACHER(tID),
constraint S_ID_FK foreign key(S_ID) references STUDENT(sID)
);

insert into TEACHER_STUDENT VALUES(1,1);
show create table TEACHER;

-- 关联查询
-- 内连接
select * from Person p inner join pets s on p.personID=s.personID;
-- 列别名和表的别名
select p.personID as id,p.FirstName,s.petID as petsId,s.`name` from Person p INNER JOIN pets s on p.personID=s.personID;
-- 隐式连接
select * from Person p,pets s where p.personID=s.personID;

-- 左外连接
select * from pets s LEFT JOIN Person p on p.personID=s.personID;

-- 右外连接
select * from pets s RIGHT JOIN Person p on p.personID=s.personID;

-- 子查询
select * from pets where personID=(select personID from Person where FirstName like 'w%');

-- group by 和常用的统计函数
select * from Person;
select count(*) from Person;-- 统计总行数
select count(*) from Person where Age>=20;

select count(Age),Age,max(score) from Person group by Age;
-- HAVING 针对统计后的结果进行操作、筛选过滤(where不行)
-- HAVING和where均可实现过滤，但在having可以使用合计函数,having通常跟在group by后，它作用于组。

select count(Age) as num,Age,max(score) from Person group by Age having num>=2;

-- where 在group by之前，order by在最后
select count(Age) as num,Age,max(score) from Person where Age>20 group by Age having num>=2;

-- 索引
show index from Person;

-- 注意执行的是cmd命令
-- mysqldump -u root -p test>test.sql 数据库备份
-- mysql -u root -p test<test.sql 数据库恢复
