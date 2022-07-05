#include "workerManager.h"
#include "employee.h"
#include "manager.h"
#include "boss.h"

workerManager::workerManager()
{
	ifstream ifs;
	ifs.open(FILENAME, ios::in);
	
	//�ļ�������
	if (!ifs.is_open())
	{
		cout << "�ļ�������" << endl;
		//��ʼ������
		this->m_worker_num = 0;
		this->m_worker_array = NULL;
		this->m_is_empty = true;
		ifs.close();
		return;
	}
	
	//�ļ����ڵ�Ϊ��
	char c;
	ifs >> c;
	if (ifs.eof())
	{
		cout << "�ļ�Ϊ��" << endl;
		//��ʼ������
		this->m_worker_num = 0;
		this->m_worker_array = NULL;
		this->m_is_empty = true;
		ifs.close();
		return;
	}

	//�ļ������Ҳ�Ϊ��
	int num = this->get_worker_num();
	//cout << "��ʼ����ʾ����ǰ����ȷ�Ǽ�Ա������Ϊ" << num << endl;
	this->m_worker_num = num;
	this->m_worker_array = new worker * [m_worker_num];
	this->init_worker();
	
	/*for (int i = 0; i < m_worker_num; i++)
	{
		cout << "ְ���ţ� " << this->m_worker_array[i]->m_ID
			<< "\tְ�������� " << this->m_worker_array[i]->m_name
			<< "\t\t���ű�ţ� " << this->m_worker_array[i]->m_deptID << endl;
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
			//cout << "���ļ��ж�ȡ��һ������Ա�����ű����Ϣ" << endl;
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
	cout << "*********  ��ӭʹ��ְ������ϵͳ�� **********" << endl;
	cout << "*************  0.�˳��������  *************" << endl;
	cout << "*************  1.����ְ����Ϣ  *************" << endl;
	cout << "*************  2.��ʾְ����Ϣ  *************" << endl;
	cout << "*************  3.ɾ����ְְ��  *************" << endl;
	cout << "*************  4.�޸�ְ����Ϣ  *************" << endl;
	cout << "*************  5.����ְ����Ϣ  *************" << endl;
	cout << "*************  6.���ձ������  *************" << endl;
	cout << "*************  7.��������ĵ�  *************" << endl;
	cout << "********************************************" << endl;
	cout << endl;
}


void workerManager::exit_system()
{
	cout << "��ӭ�´�������" << endl;
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
	cout << "���������Ա����������" << endl;
	int add_num = 0;
	cin >> add_num;
	if (add_num > 0)
	{
		int new_size = this->m_worker_num + add_num;
		worker** new_array = new worker * [new_size];
		//ԭ�������ݸ��Ƶ���������
		if (this->m_worker_array != NULL)
		{
			for (int i = 0; i < m_worker_num; i++)
			{
				new_array[i] = this->m_worker_array[i];
			}
		}

		//��Ա����¼
		for (int i = 0; i < add_num; i++)
		{
			int ID;
			string name;
			int dept_select;
			cout << "�������" << i + 1 << "����Ա�����" << endl;
			cin >> ID;
			while (this->worker_is_exist(ID) != -1)
			{
				cout << "�Ѵ�����ְͬ����ţ����������룺" << endl;
				cin >> ID;
			}
			
			
			cout << "�������" << i + 1 << "����Ա������" << endl;
			cin >> name;

			cout << "��ѡ���ְ����λ" << endl;
			cout << "1����ְͨ��" << endl;
			cout << "2������" << endl;
			cout << "3���ϰ�" << endl;
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
		
		//�ͷ�ԭ������ռ�
		delete[] this->m_worker_array;
		//����������ռ��ָ��
		this->m_worker_array = new_array;
		//�����Ѽ�¼Ա���ĸ���
		this->m_worker_num = new_size;
		//���µǼ�Ա����Ϊ�ձ�־
		this->m_is_empty = false;

		cout << "�ɹ����" << add_num << "��Ա��" << endl;

		//�������ݵ��ļ���
		this->save();
	}
	else
	{
		cout << "�������Ա�������Ƿ�" << endl;
	}

	//��������� �������ص��ϼ�Ŀ¼
	system("pause");
	system("cls");
}

void workerManager::show_worker_info()
{
	if (this->m_is_empty)
	{
		cout << "�ļ������ڻ��¼Ϊ��" << endl;
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
		cout << "�ļ������ڻ��¼Ϊ�գ�" << endl;
	}
	else
	{
		cout << "��������Ҫɾ����ְ���ţ�" << endl;
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
			//ɾ��������ͬ�����ļ���
			this->save();
			cout << "ɾ���ɹ���" << endl;
		}
		else
		{
			cout << "ɾ��ʧ�ܣ�δ�ҵ���Ա����" << endl;
		}
	}
	system("pause");
	system("cls");
}

void workerManager::modify_worker_info()
{
	if (this->m_is_empty)
	{
		cout << "�ļ������ڻ��¼Ϊ�գ�" << endl;
	}
	else
	{
		cout << "��������Ҫ�޸ĵ�ְ����ţ�" << endl;
		int id;
		cin >> id;

		int ret = this->worker_is_exist(id);
		if (ret != -1) 
		{
			delete this->m_worker_array[ret];

			int new_id;
			string new_name;
			int new_dept_select;
			cout << "�鵽" << id << "��ְ������������ְ����ţ�" << endl;
			cin >> new_id;
			cout << "��������ְ��������" << endl;
			cin >> new_name;

			cout << "��������ְ��ְλ��" << endl;
			cout << "1����ְͨ��" << endl;
			cout << "2������" << endl;
			cout << "3���ϰ�" << endl;
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
			cout << "�޸ĳɹ���" << endl;

			//�������ݺ�ͬ�����ļ���
			this->save();

		}
		else
		{
			cout << "�޸�ʧ�ܣ���ְ�������ڣ�" << endl;
		}
	}
	
	system("pause");
	system("cls");
}

void workerManager::find_worker()
{
	if (this->m_is_empty)
	{
		cout << "�ļ������ڻ��¼Ϊ�գ�" << endl;
	}
	else
	{
		cout << "��ѡ����ҷ�ʽ��" << endl;
		cout << "1.����ְ����Ų���" << endl;
		cout << "2.����ְ����������" << endl;
		int select;
		cin >> select;
		if (1 == select)
		{
			int id;
			cout << "��������Ҫ���ҵ�ְ�����" << endl;
			cin >> id;
			int ret = this->worker_is_exist(id);
			if (ret == -1)
			{
				cout << "����ʧ�ܣ��ñ�ŵ�ְ��δ��¼��" << endl;
			}
			else
			{
				cout << "���ҳɹ�����ְ����Ϣ���£�" << endl;
				this->m_worker_array[ret]->show_info();
			}
		}
		else if (2 == select)
		{
			string name;
			cout << "��������Ҫ���ҵ�ְ������" << endl;
			cin >> name;

			int flag = false;
			for (int i = 0; i < this->m_worker_num; i++)
			{
				if (this->m_worker_array[i]->m_name == name)
				{
					cout << "���ҳɹ���ְ�����Ϊ" << this->m_worker_array[i]->m_ID
						<< "��ְ����Ϣ���£�" << endl;
					this->m_worker_array[i]->show_info();
					flag = true;
				}
			}
			if (flag == false)
			{
				cout << "����ʧ�ܣ���������ְ��δ��¼��" << endl;
			}
		}
		else
		{
			cout << "�����ѡ��Ƿ�" << endl;
		}
	}
	system("pause");
	system("cls");
}

void workerManager::sort_worker()
{
	if (this->m_is_empty)
	{
		cout << "�ļ������ڻ��¼Ϊ�գ�" << endl;
		system("pause");
		system("cls");
	}
	else
	{
		cout << "��ѡ������ʽ��" << endl;
		cout << "1.����ְ�������������" << endl;
		cout << "2.����ְ����Ž�������" << endl;
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
					cout << "�����ѡ��Ƿ�" << endl;
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
		cout << "����ɹ������������£�" << endl;
		this->save(); 
		this->show_worker_info();
	}
}

void workerManager::clean_file()
{
	cout << "�Ƿ�ȷ����գ�" << endl;
	cout << "1.ȷ��" << endl;
	cout << "2.ȡ��" << endl;
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
		cout << "����ļ��ɹ���" << endl;
	}
	system("pause");
	system("cls");
}