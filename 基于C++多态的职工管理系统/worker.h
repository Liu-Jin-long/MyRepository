#pragma once
#include <iostream>
#include <string>
using namespace std;

class worker
{
public:
	//显示职工个人信息
	virtual void show_info() = 0;

	//获取岗位名称
	virtual string get_dept_name() = 0;



	int m_ID;//职工编号
	string m_name;//职工姓名
	int m_deptID;//职工所在部门名称编号
};