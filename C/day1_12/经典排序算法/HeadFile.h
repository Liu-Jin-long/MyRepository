#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#define SWAP(a, b) {int temp=a;a=b;b=temp;}
#define N 10
#define M 100


typedef int ElemType;
void print(ElemType a[]);

void Bubble_Sort(ElemType a[], int len);
void Select_Sort(ElemType a[], int len);
void Insert_Sort(ElemType a[], int len);
ElemType Partition(ElemType a[], int low, int high);
void Quick_Sort(ElemType a[], int low, int high);

void Adjust_Heap(ElemType a[], int dad, int len);
void Heap_Sort(ElemType a[], int len);
void binary_merge(ElemType a[], int low, int mid, int high);
void Merge_Sort(ElemType a[], int low, int high);
void Counting_Sort(ElemType a[]);


