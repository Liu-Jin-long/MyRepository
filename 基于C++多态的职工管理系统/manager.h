#pragma once
#include "worker.h"
#include <iostream>
using namespace std;

class manager :public worker
{
public:
	//��ͨ�����캯��
	manager(int id, string name, int dept_id);

	//��ʾ���������Ϣ
	void show_info();

	//��ȡ�����λ����
	string get_dept_name();

};
