#pragma once
#include "worker.h"
#include <iostream>
using namespace std;

class manager :public worker
{
public:
	//普通经理构造函数
	manager(int id, string name, int dept_id);

	//显示经理个人信息
	void show_info();

	//获取经理岗位名称
	string get_dept_name();

};
