#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>




typedef struct
{
	int num;
	char name[20];
	char sex;
	int course1;
	int course2;
	int course3;
}student;

typedef struct LNode//Á´±í½áµã
{
	int Total_Score;
	student stu;
	struct LNode* next;
}LNode, * LinkList;

void print(LinkList L, FILE* fp);
void print_linkList(LinkList L);

void Order_Insert(LinkList* head, LinkList* tail, int num, char name[], char sex, int course1, int course2, int course3, int Total_Score);
