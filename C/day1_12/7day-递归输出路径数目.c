#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>



int path_number(int m, int n)
{
	if (0 == m && 0 == n) return 0;//����(0,0)->(0,0)
	if (0 == m || 0 == n) return 1;//(0,0)->(m,0)ֻ��һ��·��  (0,0)->(0,n)ֻ��һ��·��
	else return path_number(m - 1, n) + path_number(m, n - 1);
}

void main()
{
	int m, n;
	scanf("%d%d", &m, &n);
	printf("�ӣ�0��0������%d,%d����·����%d��", m, n, path_number(m, n));
}