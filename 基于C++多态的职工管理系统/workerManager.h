#pragma once
#include <iostream>
using namespace std;
#include "worker.h"
#include <fstream>
#define FILENAME "File.txt"

class workerManager
{
public:	
	//�ѵǼǵ�Ա������
	int m_worker_num;

	//Ա��ָ�������
	worker** m_worker_array;
	
	//�ļ��Ƿ�Ϊ�ձ�־  �����ںʹ��ڵ������ݶ�Ϊtrue
	bool m_is_empty;

	//��ȡ�ļ����ѵǼ�Ա������
	int get_worker_num();

	//�ж�Ա���Ƿ����  ���ڷ���Ա���������е��±꣬�����ڷ���-1
	int worker_is_exist(int id);

	//��ʼ��Ա��
	void init_worker();
	
	//���캯��
	workerManager();

	//��������
	~workerManager();

	//��ʾ�˵�
	void show_menu();

	//����Ǽ�Ա����Ϣ�ļ�
	void save();

	//0.�˳��������
	void exit_system();

	//1.����ְ����Ϣ
	void add_worker();

	//2.��ʾְ����Ϣ
	void show_worker_info();
	
	//3.ɾ����ְְ��
	void delete_worker();
	
	//4.�޸�ְ����Ϣ
	void modify_worker_info();

	//5.����ְ����Ϣ
	void find_worker();

	//6.���ձ������
	void sort_worker();
	
	//7.��������ĵ�
	void clean_file();
};	