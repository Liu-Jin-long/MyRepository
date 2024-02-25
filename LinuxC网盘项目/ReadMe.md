# LinuxC网盘小项目

## 项目介绍

全部使用LinuxC编写，模仿当前主流网盘的功能并实现，可以为用户提供数据共享服务平台和搭建个人私有网盘服务使用，轻松实现数据共享。无GUI，全程通过命令进行交互。本项目提供用户注册、登录，查看、删除、创建目录、上传、下载、(上传)秒传、(下载)断点续传、客户端操作日志记录等功能。使用socket网络编程实现服务器和客户端之间的通信、并设计了简单的应用层私有传输协议解决通信双方收发数据速度不一致的问题，保证数据的正确发送和接收，使用零拷贝技术优化文件收发提高性能。使用epoll IO多路复用监控和响应多个客户端的请求，利用线程池处理耗时请求(gets和puts)实现命令分离，使用token机制保持连接避免重复密码验证。使用mysql数据库进行记录和管理用户信息、网盘服务器文件池信息、用户的上传的所有文件信息以及用户操作的日志信息。在用户文件表中使用虚拟文件系统简化存储逻辑并节省数据库存储空间、使用文件池表管理和保存所有文件、通过MD5值和文件大小确保唯一性，同时避免产生冗余存储。另外服务器使用轮盘法实现客户端超时无请求发送就主动关闭连接，节省服务器资源。

支持命令：

- ls  显示当前网盘目录的所有文件和文件夹   用法：支持ls、ls /和ls dir
- cd  切换网盘路径      用法：支持cd . 、cd .. 、cd / 和cd dir
- pwd 显示当前网盘路径  用法：pwd
- rm 删除当前网盘路径的文件或文件夹(文件夹会递归删除里面的所有文件)  用法：rm file或rm dir
- mkdir 在当前网盘路径下创建文件夹  用法：mkdir dir_name
- gets 从网盘下载文件到本地路径 用法：gets file
- puts 从本地路径上传文件到网盘服务器  用法：puts file

## 项目使用

include 头文件      src C源文件       server 服务器运行       client 客户端运行

#### 构建数据

登录数据库 

```
mysql -u username -p password
```

创建网盘数据库 

```
create database netdisk;
```

导入sql文件 

```
mysql -u username -p password NetworkDiskDatabase < NetworkDiskDatabase.sql
```

#### 配置文件

配置文件就是include/config.h，直接修改宏即可

注意修改数据库用户名和密码

#### 编译和运行

1、进入server和client目录  使用make命令进行编译生成可执行文件server （偷懒写法src C源文件都编译成目标文件到当前目录🤣）

注：可以使用make clean清理删除生成的.o文件和可执行文件      还可以使用make rebuild重新构建（其实就是make clean;make的功能）

```
如果找不到<mysql/mysql.h>
sudo apt install libmysqlclient-dev
```

2、在各子目录下./server和./client运行