#include "employee.h"


employee::employee(int id, string name, int dept_id)
{
	this->m_ID = id;
	this->m_name = name;
	this->m_deptID = dept_id;
}

void employee::show_info()
{
	cout << "职工编号：" << this->m_ID
		<< "\t职工姓名：" << this->m_name
		<< "\t岗位：" << this->get_dept_name()
		<< "\t岗位职责：完成经理交给的任务" << endl;
}

string employee::get_dept_name()
{
	return string("普通员工");
}