#include "boss.h"


boss::boss(int id, string name, int dept_id)
{
	this->m_ID = id;
	this->m_name = name;
	this->m_deptID = dept_id;
}

void boss::show_info()
{
	cout << "ְ����ţ�" << this->m_ID
		<< "\tְ��������" << this->m_name
		<< "\t��λ��" << this->get_dept_name()
		<< "\t��λְ�𣺹���˾��������" << endl;
}

string boss::get_dept_name()
{
	return string("�ϰ�");
}