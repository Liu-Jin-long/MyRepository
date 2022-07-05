#include "HeadFile.h"

void print(ElemType a[])
{
	for (int i = 0; i < N; i++) printf("%3d", a[i]);
	putchar('\n');
}


void Bubble_Sort(ElemType a[], int len)
{
	int flag;
	for (int i = len - 1; i > 0; i--)//N-1趟，最大的放右边
	{
		flag = 0;
		for (int j = 0; j < i;j++)
		{
			if (a[j] > a[j + 1])
			{
				SWAP(a[j], a[j + 1]);
				flag = 1;
			}
		}
		if (flag == 0) return;//若有一趟排序为交换元素，说明已有序，直接结束冒泡排序
	}
}


void Select_Sort(ElemType a[], int len)
{
	ElemType min;
	for (int i = 0; i <len -1; i++)
	{
		min = i;
		for (int j = i + 1; j <len;j++)
		{
			if(a[j] < a[min]) min = j;
		}
		SWAP(a[i], a[min]);
	}
}

void Insert_Sort(ElemType a[], int len)
{
	ElemType temp;
	int j;
	for (int i = 1; i < len; i++)
	{
		temp = a[i];
		for (j = i - 1; j >= 0; j--)
		{
			if (a[j] > temp)
			{
				a[j + 1] = a[j];
			}
			else break;
		}
		a[j + 1] = temp;
	}
}



ElemType Partition(ElemType a[], int low, int high)
{
	ElemType pivot = a[low];//挖坑法
	while (low < high)
	{
		while (a[high] >= pivot && high > low) high--;
		a[low] = a[high];
		while (a[low] <= pivot && high > low) low++;
		a[high] = a[low];
	}
	a[low] = pivot;
	return low;

	//int i = low;//交换法
	//for (int j = low; j < high;j++)
	//{
	//	if (a[j] < a[high])//a[high]作分割值
	//	{
	//		SWAP(a[j], a[i]);
	//		i++;
	//	}
	//}
	//SWAP(a[i], a[high]);
	//return i;
}





void Quick_Sort(ElemType a[], int low, int high)
{
	
	ElemType pivot ;
	if (low < high)
	{
		pivot = Partition(a, low, high);
		Quick_Sort(a, low, pivot - 1);
		Quick_Sort(a, pivot + 1, high);
	}
}



void Adjust_Heap(ElemType a[], int dad, int len)
{
	int son = dad * 2 + 1;//左孩子
	while (son < len)
	{
		if (son + 1 < len && a[son] < a[son + 1])
		{
			son++;
		}
		if (a[son] > a[dad])
		{
			SWAP(a[son], a[dad]);
			dad = son;
			son = dad * 2 + 1;
		}
		else break;
	}
}

void Heap_Sort(ElemType a[], int len)
{
	int i;
	for (i = len >> 1 - 1; i >= 0; i--)//初始调整堆
	{
		Adjust_Heap(a, i, len);
	}
	SWAP(a[0], a[len - 1]);//交换首尾元素
	for (i = len - 1; i > 1; i--)
	{
		Adjust_Heap(a, 0, i);
		SWAP(a[0], a[i - 1]);//每次调整堆，交换首尾元素
	}

}

void binary_merge(ElemType a[], int low, int mid, int high)
{
	int b[N];
	int i, j, k = low;
	for (int h = low; h <= high; h++)//数据复制一份
	{
		b[h] = a[h];
	}
	for (i = low, j = mid + 1; i <= mid && j <= high; k++)
	{
		if (b[i] <= b[j])
		{
			a[k] = b[i];
			i++;
		}
		else
		{
			a[k] = b[j];
			j++;
		}
	}
	while (i <= mid) a[k++] = b[i++];
	while (j <= high) a[k++] = b[j++];
}


void Merge_Sort(ElemType a[], int low, int high)
{
	if (low < high)
	{
		int mid = (low + high) / 2;
		Merge_Sort(a, low, mid);
		Merge_Sort(a, mid + 1, high);
		binary_merge(a, low, mid, high);
	}
}


void Counting_Sort(ElemType a[])
{
	ElemType amount[M] = { 0 };
	for (int i = 0; i < N; i++)
	{
		amount[a[i]]++;
	}
	int k = 0;
	for (int i = 0; i < M; i++)
	{
		int j = 0;
		while (j < amount[i])
		{
			a[k] = i;
			k++;
			j++;
		}
	}
}

