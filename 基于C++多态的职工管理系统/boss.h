#pragma once
#include "worker.h"
#include <iostream>
using namespace std;

class boss :public worker
{
public:
	//��ͨԱ�����캯��
	boss(int id, string name, int dept_id);

	//��ʾ��ͨԱ��������Ϣ
	void show_info();

	//��ȡ��ͨԱ����λ����
	string get_dept_name();

};
