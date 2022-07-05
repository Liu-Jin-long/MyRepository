#include <iostream>
using namespace std;
#include "workerManager.h"
#include "worker.h"
#include "employee.h"
#include "manager.h"
#include "boss.h"



int main()
{
	
	//实例化职工管理系统
	workerManager wm;
	int choice;
	while(true)
	{
		wm.show_menu();
		cout << "请输入您的选择：" << endl;
		cin >> choice;
		switch (choice)
		{
		case 0:
			//退出管理程序
			wm.exit_system();
			break;
		case 1:
			//增加职工信息
			wm.add_worker();
			break;
		case 2:
			//显示职工信息
			wm.show_worker_info();
			break;
		case 3:
			//删除离职职工
			wm.delete_worker();
			break;
		case 4:
			//修改职工信息
			wm.modify_worker_info();
			break;
		case 5:
			//查找职工信息
			wm.find_worker();
			break;
		case 6:
			//按照编号排序
			wm.sort_worker();
			break;
		case 7:
			//清空所有文档
			wm.clean_file();
			break;
		default:
			break;
		}
	}

	return 0;
}