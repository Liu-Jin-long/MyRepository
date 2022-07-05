#pragma once
#include <iostream>
using namespace std;
#include "worker.h"
#include <fstream>
#define FILENAME "File.txt"

class workerManager
{
public:	
	//已登记的员工人数
	int m_worker_num;

	//员工指针的数组
	worker** m_worker_array;
	
	//文件是否为空标志  不存在和存在但空数据都为true
	bool m_is_empty;

	//获取文件中已登记员工人数
	int get_worker_num();

	//判断员工是否存在  存在返回员工在数组中的下标，不存在返回-1
	int worker_is_exist(int id);

	//初始化员工
	void init_worker();
	
	//构造函数
	workerManager();

	//析构函数
	~workerManager();

	//显示菜单
	void show_menu();

	//保存登记员工信息文件
	void save();

	//0.退出管理程序
	void exit_system();

	//1.增加职工信息
	void add_worker();

	//2.显示职工信息
	void show_worker_info();
	
	//3.删除离职职工
	void delete_worker();
	
	//4.修改职工信息
	void modify_worker_info();

	//5.查找职工信息
	void find_worker();

	//6.按照编号排序
	void sort_worker();
	
	//7.清空所有文档
	void clean_file();
};	