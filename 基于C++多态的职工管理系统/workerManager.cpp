#include "workerManager.h"
#include "employee.h"
#include "manager.h"
#include "boss.h"

workerManager::workerManager()
{
	ifstream ifs;
	ifs.open(FILENAME, ios::in);
	
	//文件不存在
	if (!ifs.is_open())
	{
		cout << "文件不存在" << endl;
		//初始化属性
		this->m_worker_num = 0;
		this->m_worker_array = NULL;
		this->m_is_empty = true;
		ifs.close();
		return;
	}
	
	//文件存在但为空
	char c;
	ifs >> c;
	if (ifs.eof())
	{
		cout << "文件为空" << endl;
		//初始化属性
		this->m_worker_num = 0;
		this->m_worker_array = NULL;
		this->m_is_empty = true;
		ifs.close();
		return;
	}

	//文件存在且不为空
	int num = this->get_worker_num();
	//cout << "初始化提示：当前已正确登记员工人数为" << num << endl;
	this->m_worker_num = num;
	this->m_worker_array = new worker * [m_worker_num];
	this->init_worker();
	
	/*for (int i = 0; i < m_worker_num; i++)
	{
		cout << "职工号： " << this->m_worker_array[i]->m_ID
			<< "\t职工姓名： " << this->m_worker_array[i]->m_name
			<< "\t\t部门编号： " << this->m_worker_array[i]->m_deptID << endl;
	}*/
}

int workerManager::worker_is_exist(int id)
{
	int index = -1;
	for (int i = 0; i < this->m_worker_num; i++)
	{
		if (this->m_worker_array[i]->m_ID == id)
		{
			index = i;
			break;
		}
	}
	return index;
}

workerManager::~workerManager()
{
	if (this->m_worker_array != NULL)
	{
		for (int i = 0; i < this->m_worker_num; i++)
		{
			if (this->m_worker_array[i] != NULL)
			{
				delete this->m_worker_array[i];
			}
		}
		
		delete[] m_worker_array;
		this->m_worker_array = NULL;
	}
}

void workerManager::init_worker()
{
	ifstream ifs;
	ifs.open(FILENAME, ios::in);
	
	int id;
	string name;
	int dept_id;

	int i = 0;
	while (ifs >> id && ifs >> name && ifs >> dept_id)
	{
		worker* file_worker;
		if (1 == dept_id)
		{
			file_worker = new employee(id, name, dept_id);
		}
		else if (2 == dept_id)
		{
			file_worker = new manager(id, name, dept_id);
		}
		else if (3 == dept_id)
		{
			file_worker = new boss(id, name, dept_id);
		}
		else
		{
			//cout << "从文件中读取到一条错误员工部门编号信息" << endl;
			continue;
		}
		this->m_worker_array[i] = file_worker;
		i++;
	}
}

int workerManager::get_worker_num()
{
	ifstream ifs;
	ifs.open(FILENAME, ios::in);
	int id;
	string name;
	int dept_id;

	int num = 0;
	while (ifs >> id && ifs >> name && ifs >> dept_id)
	{
		if (1 == dept_id || 2 == dept_id || 3 == dept_id) num++;
	}
	return num;
}

void workerManager::show_menu()
{
	cout << "********************************************" << endl;
	cout << "*********  欢迎使用职工管理系统！ **********" << endl;
	cout << "*************  0.退出管理程序  *************" << endl;
	cout << "*************  1.增加职工信息  *************" << endl;
	cout << "*************  2.显示职工信息  *************" << endl;
	cout << "*************  3.删除离职职工  *************" << endl;
	cout << "*************  4.修改职工信息  *************" << endl;
	cout << "*************  5.查找职工信息  *************" << endl;
	cout << "*************  6.按照编号排序  *************" << endl;
	cout << "*************  7.清空所有文档  *************" << endl;
	cout << "********************************************" << endl;
	cout << endl;
}


void workerManager::exit_system()
{
	cout << "欢迎下次再来！" << endl;
	exit(0);
}


void workerManager::save()
{
	ofstream ofs;
	ofs.open(FILENAME, ios::out);

	for (int i = 0; i < this->m_worker_num; i++)
	{
		ofs << this->m_worker_array[i]->m_ID << " "
			<< this->m_worker_array[i]->m_name << " "
			<< this->m_worker_array[i]->m_deptID << endl;
	}

	ofs.close();
}

void workerManager::add_worker()
{
	cout << "请输入添加员工的数量：" << endl;
	int add_num = 0;
	cin >> add_num;
	if (add_num > 0)
	{
		int new_size = this->m_worker_num + add_num;
		worker** new_array = new worker * [new_size];
		//原数组内容复制到新数组中
		if (this->m_worker_array != NULL)
		{
			for (int i = 0; i < m_worker_num; i++)
			{
				new_array[i] = this->m_worker_array[i];
			}
		}

		//新员工记录
		for (int i = 0; i < add_num; i++)
		{
			int ID;
			string name;
			int dept_select;
			cout << "请输入第" << i + 1 << "个新员工编号" << endl;
			cin >> ID;
			while (this->worker_is_exist(ID) != -1)
			{
				cout << "已存在相同职工编号，请重新输入：" << endl;
				cin >> ID;
			}
			
			
			cout << "请输入第" << i + 1 << "个新员工姓名" << endl;
			cin >> name;

			cout << "请选择该职工岗位" << endl;
			cout << "1、普通职工" << endl;
			cout << "2、经理" << endl;
			cout << "3、老板" << endl;
			cin >> dept_select;

			worker* new_worker = NULL;
			switch (dept_select)
			{
			case 1:
				new_worker = new employee(ID, name, 1);
				break;
			case 2:
				new_worker = new manager(ID, name, 2);
				break;
			case 3:
				new_worker = new boss(ID, name, 3);
				break;
			default:
				break;
			}

			new_array[this->m_worker_num + i] = new_worker;
		}
		
		//释放原有数组空间
		delete[] this->m_worker_array;
		//更改新数组空间的指向
		this->m_worker_array = new_array;
		//更新已记录员工的个数
		this->m_worker_num = new_size;
		//更新登记员工不为空标志
		this->m_is_empty = false;

		cout << "成功添加" << add_num << "名员工" << endl;

		//保存数据到文件中
		this->save();
	}
	else
	{
		cout << "输入添加员工数量非法" << endl;
	}

	//按任意键后 清屏返回到上级目录
	system("pause");
	system("cls");
}

void workerManager::show_worker_info()
{
	if (this->m_is_empty)
	{
		cout << "文件不存在或记录为空" << endl;
	}
	else
	{
		for (int i = 0; i < this->m_worker_num; i++)
		{
			this->m_worker_array[i]->show_info();
		}
	}
	system("pause");
	system("cls");
}

void workerManager::delete_worker()
{
	if (this->m_is_empty)
	{
		cout << "文件不存在或记录为空！" << endl;
	}
	else
	{
		cout << "请输入想要删除的职工号：" << endl;
		int id;
		cin >> id;
		int index = this->worker_is_exist(id);

		if (index != -1)
		{
			for (int i = index; i < this->m_worker_num; i++)
			{
				this->m_worker_array[i] = this->m_worker_array[i + 1];
			}
			this->m_worker_num--;
			//删除后数据同步到文件中
			this->save();
			cout << "删除成功！" << endl;
		}
		else
		{
			cout << "删除失败，未找到该员工！" << endl;
		}
	}
	system("pause");
	system("cls");
}

void workerManager::modify_worker_info()
{
	if (this->m_is_empty)
	{
		cout << "文件不存在或记录为空！" << endl;
	}
	else
	{
		cout << "请输入所要修改的职工编号：" << endl;
		int id;
		cin >> id;

		int ret = this->worker_is_exist(id);
		if (ret != -1) 
		{
			delete this->m_worker_array[ret];

			int new_id;
			string new_name;
			int new_dept_select;
			cout << "查到" << id << "号职工，请输入新职工编号：" << endl;
			cin >> new_id;
			cout << "请输入新职工姓名：" << endl;
			cin >> new_name;

			cout << "请输入新职工职位：" << endl;
			cout << "1、普通职工" << endl;
			cout << "2、经理" << endl;
			cout << "3、老板" << endl;
			cin >> new_dept_select;

			worker* new_worker = NULL;
			switch (new_dept_select)
			{
			case 1:
				new_worker = new employee(new_id, new_name, 1);
				break;
			case 2:
				new_worker = new manager(new_id, new_name, 2);
				break;
			case 3:
				new_worker = new boss(new_id, new_name, 3);
				break;
			default:
				break;
			}
			
			this->m_worker_array[ret] = new_worker;
			cout << "修改成功！" << endl;

			//更新数据后同步到文件中
			this->save();

		}
		else
		{
			cout << "修改失败，该职工不存在！" << endl;
		}
	}
	
	system("pause");
	system("cls");
}

void workerManager::find_worker()
{
	if (this->m_is_empty)
	{
		cout << "文件不存在或记录为空！" << endl;
	}
	else
	{
		cout << "请选择查找方式：" << endl;
		cout << "1.按照职工编号查找" << endl;
		cout << "2.按照职工姓名查找" << endl;
		int select;
		cin >> select;
		if (1 == select)
		{
			int id;
			cout << "请输入所要查找的职工编号" << endl;
			cin >> id;
			int ret = this->worker_is_exist(id);
			if (ret == -1)
			{
				cout << "查找失败，该编号的职工未记录！" << endl;
			}
			else
			{
				cout << "查找成功！该职工信息如下：" << endl;
				this->m_worker_array[ret]->show_info();
			}
		}
		else if (2 == select)
		{
			string name;
			cout << "请输入所要查找的职工姓名" << endl;
			cin >> name;

			int flag = false;
			for (int i = 0; i < this->m_worker_num; i++)
			{
				if (this->m_worker_array[i]->m_name == name)
				{
					cout << "查找成功！职工编号为" << this->m_worker_array[i]->m_ID
						<< "的职工信息如下：" << endl;
					this->m_worker_array[i]->show_info();
					flag = true;
				}
			}
			if (flag == false)
			{
				cout << "查找失败，该姓名的职工未记录！" << endl;
			}
		}
		else
		{
			cout << "输入的选项非法" << endl;
		}
	}
	system("pause");
	system("cls");
}

void workerManager::sort_worker()
{
	if (this->m_is_empty)
	{
		cout << "文件不存在或记录为空！" << endl;
		system("pause");
		system("cls");
	}
	else
	{
		cout << "请选择排序方式：" << endl;
		cout << "1.按照职工编号升序排列" << endl;
		cout << "2.按照职工编号降序排列" << endl;
		int select;
		cin >> select;

		for (int i = 0; i < this->m_worker_num; i++)
		{
			int min_or_max = i;
			for (int j = i + 1; j < this->m_worker_num; j++)
			{
				if (1 == select)
				{
					if (this->m_worker_array[j]->m_ID < this->m_worker_array[min_or_max]->m_ID)
					{
						min_or_max = j;
					}
				}
				else if (2 == select)
				{
					if (this->m_worker_array[j]->m_ID > this->m_worker_array[min_or_max]->m_ID)
					{
						min_or_max = j;
					}
				}
				else
				{
					cout << "输入的选项非法" << endl;
					break;
				}
			}
			if (min_or_max != i)
			{
				worker* temp = this->m_worker_array[min_or_max];
				this->m_worker_array[min_or_max] = this->m_worker_array[i];
				this->m_worker_array[i] = temp;
			}
		}
		cout << "排序成功！排序结果如下：" << endl;
		this->save(); 
		this->show_worker_info();
	}
}

void workerManager::clean_file()
{
	cout << "是否确认清空？" << endl;
	cout << "1.确认" << endl;
	cout << "2.取消" << endl;
	int select;
	cin >> select;
	if (1 == select)
	{
		ofstream ofs;
		ofs.open(FILENAME, ios::trunc);
		ofs.close();
		if (this->m_worker_array != NULL)
		{
			for (int i = 0; i < this->m_worker_num; i++)
			{
				if (m_worker_array[i] != NULL)
				{
					delete m_worker_array[i];
				}
			}

			this->m_worker_num = 0;
			delete[] this->m_worker_array;
			this->m_worker_array = NULL;
			this->m_is_empty = true;
		}
		cout << "清空文件成功！" << endl;
	}
	system("pause");
	system("cls");
}