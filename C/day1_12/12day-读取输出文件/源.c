#include "HeadFile.h"

void main()
{
	FILE* fp=fopen("File.txt","r+");
	student stu;
	LinkList head=NULL, tail=NULL;

	while (fscanf(fp, "%d%s    %c%d%d%d", &stu.num, stu.name, &stu.sex, &stu.course1, &stu.course2, &stu.course3) != EOF)
	{
		Order_Insert(&head, &tail, stu.num, stu.name, stu.sex, stu.course1, stu.course2, stu.course3, stu.course1 + stu.course2 + stu.course3);
	}
	print_linkList(head);//��ӡ��һ����������ѧ����Ϣ���ܷ�
	
	fseek(fp, 0, SEEK_SET);
	print(head, fp);//����д��Դ�ļ�
	
	fclose(fp);
}