#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int stu_num;
	char name[20];
	int course1;
	int course2;
	int course3;
}student;

void main()
{
	student stu[5];
	int total[5] = {0};
	for (int i = 0; i < 5; i++)
	{
		scanf("%d %s %d %d %d", &stu[i].stu_num, &stu[i].name, &stu[i].course1, &stu[i].course2, &stu[i].course3);
		total[i] = stu[i].course1 + stu[i].course2 + stu[i].course3;
	}
	
	for (int i = 1; i < 5; i++)
	{
		student temp;
		int n_temp;
		for (int j = 4 ; j >0; j--)
		{
			if (stu[j].stu_num < stu[j - 1].stu_num)
			{
				temp = stu[j];
				stu[j] = stu[j - 1];
				stu[j - 1] = temp;
				n_temp = total[j];
				total[j] = total[j - 1];
				total[j - 1] = n_temp;
			}
		}
	}
	printf("学号递增输出全部学生信息\n");
	for (int i = 0; i < 5; i++)
	{
		printf("%d %s %d %d %d %d\n", stu[i].stu_num, stu[i].name, stu[i].course1, stu[i].course2, stu[i].course3, total[i]);
	}
	
	printf("\n");


	for (int i = 1; i < 5; i++) total[i] = stu[i].course1 + stu[i].course2 + stu[i].course3;
	for (int i = 1; i < 5; i++)
	{
		student temp;
		int n_temp;
		for (int j = 4; j > 0; j--)
		{
			if (total[j] < total[j-1])
			{
				n_temp = total[j];
				total[j] = total[j - 1];
				total[j - 1] = n_temp;
				temp = stu[j];
				stu[j] = stu[j - 1];
				stu[j - 1] = temp;
			}
		}
	}
	printf("按照总分输出学生排名\n");
	for (int i = 4, j = 1; i >= 0; i--)
	{
		printf("%d %s\n", j, stu[i].name);
		j++;
	}


	printf("\n");

	for (int i = 1; i < 5; i++)
	{
		student temp;
		for (int j = 4; j > 0; j--)
		{
			if (stu[j].course1 < stu[j - 1].course1)
			{
				temp = stu[j];
				stu[j] = stu[j - 1];
				stu[j - 1] = temp;
			}
		}
	}
	printf("课程1最高分的学生的信息是\n");
	printf("%d %s %d %d %d\n", stu[4].stu_num, stu[4].name, stu[4].course1, stu[4].course2, stu[4].course3);
	
	printf("\n");
	
	for (int i = 1; i < 5; i++)
	{
		student temp;
		for (int j = 4; j > 0; j--)
		{
			if (stu[j].course2 < stu[j - 1].course2)
			{
				temp = stu[j];
				stu[j] = stu[j - 1];
				stu[j - 1] = temp;
			}
		}
	}
	printf("课程2最高分的学生的信息是\n");
	printf("%d %s %d %d %d\n", stu[4].stu_num, stu[4].name, stu[4].course1, stu[4].course2, stu[4].course3);

	printf("\n");

	for (int i = 1; i < 5; i++)
	{
		student temp;
		for (int j = 4; j > 0; j--)
		{
			if (stu[j].course3 < stu[j - 1].course3)
			{
				temp = stu[j];
				stu[j] = stu[j - 1];
				stu[j - 1] = temp;
			}
		}
	}
	printf("课程3最高分的学生的信息是\n");
	printf("%d %s %d %d %d\n", stu[4].stu_num, stu[4].name, stu[4].course1, stu[4].course2, stu[4].course3);

	printf("\n");
	
	int course_total = 0, course_tota2 = 0, course_tota3 = 0;
	for (int i = 0; i < 5; i++)
	{
		course_total += stu[i].course1;
		course_tota2 += stu[i].course2;
		course_tota3 += stu[i].course3;
	}

	printf("课程1平均分是%5.2f\n", (float)course_total / 5);
	printf("课程1平均分是%5.2f\n", (float)course_tota2 / 5);
	printf("课程1平均分是%5.2f\n", (float)course_tota2 / 5);

	
}
