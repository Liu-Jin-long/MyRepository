#include "manager.h"


manager::manager(int id, string name, int dept_id)
{
	this->m_ID = id;
	this->m_name = name;
	this->m_deptID = dept_id;
}

void manager::show_info()
{
	cout << "职工编号：" << this->m_ID
		<< "\t职工姓名：" << this->m_name
		<< "\t岗位：" << this->get_dept_name()
		<< "\t岗位职责：完成老板交给的任务，并下发任务给员工" << endl;
}

string manager::get_dept_name()
{
	return string("经理");
}