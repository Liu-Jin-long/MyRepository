#include "boss.h"


boss::boss(int id, string name, int dept_id)
{
	this->m_ID = id;
	this->m_name = name;
	this->m_deptID = dept_id;
}

void boss::show_info()
{
	cout << "职工编号：" << this->m_ID
		<< "\t职工姓名：" << this->m_name
		<< "\t岗位：" << this->get_dept_name()
		<< "\t岗位职责：管理公司所有事务" << endl;
}

string boss::get_dept_name()
{
	return string("老板");
}