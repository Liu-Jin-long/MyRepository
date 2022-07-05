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
	print_linkList(head);//打印看一下链表各结点学生信息的总分
	
	fseek(fp, 0, SEEK_SET);
	print(head, fp);//数据写回源文件
	
	fclose(fp);
}