#include "HeadFile.h"



int compare(const void* p1, const void* p2)
{
	int* left = (int*)p1;
	int* right = (int*)p2;
	//return *left - *right;  浮点数时不能这样写
	if (*left > *right) return 1;
	else if (*left < *right) return -1;
	return 0;
}



void main()
{
	ElemType* a = (ElemType*)calloc(N, sizeof(ElemType));
	time_t start, end;//此时距1970 1月1号的秒数
	srand(time(NULL));
	for (int i = 0; i < N; i++) a[i] = rand() % M;
	print(a);
	start = time(NULL);
	Bubble_Sort(a, N);
	print(a);
	Select_Sort(a, N);
	print(a);
	Insert_Sort(a, N);
	print(a);
	Quick_Sort(a, 0, N - 1);
	print(a);

	Heap_Sort(a, N);
	print(a);
	/*Merge_Sort(a, 0, N-1);*/
	/*Counting_Sort(a);*/
	/*print(a);*/
	end = time(NULL);
	printf("start=%lld, end=%lld, use time=%lld\n", start, end, end - start);
}