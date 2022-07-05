#include <iostream>
using namespace std;
#include "workerManager.h"
#include "worker.h"
#include "employee.h"
#include "manager.h"
#include "boss.h"



int main()
{
	
	//ʵ����ְ������ϵͳ
	workerManager wm;
	int choice;
	while(true)
	{
		wm.show_menu();
		cout << "����������ѡ��" << endl;
		cin >> choice;
		switch (choice)
		{
		case 0:
			//�˳��������
			wm.exit_system();
			break;
		case 1:
			//����ְ����Ϣ
			wm.add_worker();
			break;
		case 2:
			//��ʾְ����Ϣ
			wm.show_worker_info();
			break;
		case 3:
			//ɾ����ְְ��
			wm.delete_worker();
			break;
		case 4:
			//�޸�ְ����Ϣ
			wm.modify_worker_info();
			break;
		case 5:
			//����ְ����Ϣ
			wm.find_worker();
			break;
		case 6:
			//���ձ������
			wm.sort_worker();
			break;
		case 7:
			//��������ĵ�
			wm.clean_file();
			break;
		default:
			break;
		}
	}

	return 0;
}