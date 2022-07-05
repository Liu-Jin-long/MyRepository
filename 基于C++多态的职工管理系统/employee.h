#pragma once
#include "worker.h"
#include <iostream>
using namespace std;

class employee :public worker
{
public:
	//普通员工构造函数
	employee(int id, string name, int dept_id);

	//显示普通员工个人信息
	void show_info();

	//获取普通员工岗位名称
	string get_dept_name();

};
