#pragma once
#include "worker.h"
#include <iostream>
using namespace std;

class employee :public worker
{
public:
	//��ͨԱ�����캯��
	employee(int id, string name, int dept_id);

	//��ʾ��ͨԱ��������Ϣ
	void show_info();

	//��ȡ��ͨԱ����λ����
	string get_dept_name();

};
