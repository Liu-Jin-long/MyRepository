#include "manager.h"


manager::manager(int id, string name, int dept_id)
{
	this->m_ID = id;
	this->m_name = name;
	this->m_deptID = dept_id;
}

void manager::show_info()
{
	cout << "ְ����ţ�" << this->m_ID
		<< "\tְ��������" << this->m_name
		<< "\t��λ��" << this->get_dept_name()
		<< "\t��λְ������ϰ彻�������񣬲��·������Ա��" << endl;
}

string manager::get_dept_name()
{
	return string("����");
}