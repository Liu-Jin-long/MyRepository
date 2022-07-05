# # 编写于2022年06月10日10时44分41秒
# # 循环队列
# class CircularQueue():
#     def __init__(self, MaxSixe):
#         self.MaxSize = MaxSixe
#         self.Queen = [None] * MaxSixe
#         self.rear = 0
#         self.front = 0
#
#     def enqueue(self, elem):
#         if (self.rear + 1) % self.MaxSize == self.front:
#             print('队列已满，无法入队！')
#             return False
#         self.Queen[self.rear] = elem
#         self.rear = (self.rear + 1) % self.MaxSize
#         return True
#
#     def dequeue(self):
#         if self.front == self.rear:
#             print('队列为空，无法出队')
#         elem = self.Queen[self.front]
#         self.front = (self.front + 1) % self.MaxSize
#         return elem
#
#
# cq = CircularQueue(5)
# cq.enqueue(2)
# cq.enqueue(1)
# cq.enqueue(3)
# cq.enqueue(7)
# print(cq.dequeue())
# print(cq.Queen)
#
#
# # 二叉树的层次建树，前序遍历，中序遍历，后序遍历
# class Node():
#     def __init__(self, elem=None, lchild=None, rchild=None):
#         self.elem = elem
#         self.lchild = lchild
#         self.rchild = rchild
#
#
# class BinaryTree():
#     def __init__(self):
#         self.root = None
#         self.Quenu: list[Node] = []
#
#     def build_tree(self, elem):
#         new_node = Node(elem)
#         self.Quenu.append(new_node)
#         if self.root is None:
#             self.root = new_node
#         else:
#             if self.Quenu[0].lchild is None:
#                 self.Quenu[0].lchild = new_node
#             elif self.Quenu[0].rchild is None:
#                 self.Quenu[0].rchild = new_node
#                 self.Quenu.pop(0)
#
#     def preorder(self, root: Node):
#         if root:
#             print(root.elem, end=' ')
#             self.preorder(root.lchild)
#             self.preorder(root.rchild)
#
#     def inorder(self, root: Node):
#         if root:
#             self.inorder(root.lchild)
#             print(root.elem, end=' ')
#             self.inorder(root.rchild)
#
#     def postorder(self, root: Node):
#         if root:
#             self.postorder(root.lchild)
#             self.postorder(root.rchild)
#             print(root.elem, end=' ')
#
#
# tree = BinaryTree()
# for i in 'abcdefghij':
#     tree.build_tree(i)
# tree.preorder(tree.root)
# print()
# tree.inorder(tree.root)
# print()
# tree.postorder(tree.root)
# print()

# 排序算法
import random
import time
import sys

sys.setrecursionlimit(100000)


class MySort():
    def __init__(self, num, num_range):
        self.n = num
        self.num_range = num_range
        self.arr = [random.randint(0, num_range) for i in range(num)]
        self.merge_arr = [None] * num

    def test_sort(self, sort_name, *args, **kwargs):
        # print(self.arr)
        start = time.time()
        sort_name(*args, **kwargs)
        end = time.time()
        print(end - start)
        # print(self.arr)

    # 冒泡排序
    def BubbleSort(self):
        j = self.n - 1
        while j > 0:
            i = 0
            while i < j:
                if self.arr[i] > self.arr[i + 1]:
                    self.arr[i], self.arr[i + 1] = self.arr[i + 1], self.arr[i]
                i += 1
            j -= 1

    # 选择排序
    def SelectSort(self):
        i = 0
        while i < self.n - 1:
            min_index = i
            j = i + 1
            while j < self.n:
                if self.arr[j] < self.arr[min_index]:
                    min_index = j
                j += 1
            self.arr[i], self.arr[min_index] = self.arr[min_index], self.arr[i]
            i += 1

    # 插入排序
    def InsertSort(self):
        i = 1
        while i < self.n:
            insert_data = self.arr[i]
            j = i - 1
            while j >= 0:
                if insert_data < self.arr[j]:
                    self.arr[j + 1] = self.arr[j]
                else:
                    break
                j -= 1
            self.arr[j + 1] = insert_data
            i += 1

    # 快速排序
    def Partition(self, left, right):
        pivot = j = left
        while j < right:  # pivot放置比分隔值小的元素
            if self.arr[j] < self.arr[right]:
                self.arr[j], self.arr[pivot] = self.arr[pivot], self.arr[j]
                pivot += 1
            j += 1
        self.arr[pivot], self.arr[right] = self.arr[right], self.arr[pivot]
        return pivot

    def QuickSort(self, left_index, right_index):
        if left_index < right_index:
            pivot = self.Partition(left_index, right_index)
            self.QuickSort(left_index, pivot - 1)
            self.QuickSort(pivot + 1, right_index)

    # 堆排序
    def AdjustHeap(self, index, len):
        son = index * 2 + 1
        while son < len:
            if son + 1 < len and self.arr[son] < self.arr[son + 1]:
                son += 1
            if self.arr[son] > self.arr[index]:
                self.arr[son], self.arr[index] = self.arr[index], self.arr[son]
                index = son
                son = index * 2 + 1
            else:
                break

    def HeapSort(self):
        for i in range(self.n // 2 - 1, -1, -1):
            self.AdjustHeap(i, self.n)
        self.arr[0], self.arr[self.n - 1] = self.arr[self.n - 1], self.arr[0]
        for i in range(self.n - 1, 1, -1):
            self.AdjustHeap(0, i)
            self.arr[0], self.arr[i - 1] = self.arr[i - 1], self.arr[0]

    # 归并排序
    def Merge(self, low, mid, high):
        for i in range(low, high + 1):
            self.merge_arr[i] = self.arr[i]
        i = low
        j = mid + 1
        k = low
        while i <= mid and j <= high:
            if self.merge_arr[i] < self.merge_arr[j]:
                self.arr[k] = self.merge_arr[i]
                k += 1
                i += 1
            else:
                self.arr[k] = self.merge_arr[j]
                k += 1
                j += 1
        while i <= mid:
            self.arr[k] = self.merge_arr[i]
            k += 1
            i += 1
        while j <= high:
            self.arr[k] = self.merge_arr[j]
            k += 1
            j += 1

    def MergeSort(self, low, high):
        if low < high:
            mid = (low + high) // 2
            self.MergeSort(low, mid)
            self.MergeSort(mid + 1, high)
            self.Merge(low, mid, high)

    # 计数排序
    def CountingSort(self):
        count_arr = [0] * (self.num_range + 1)
        for i in self.arr:
            count_arr[i] += 1
        k = 0
        for i in range(0, self.num_range + 1):
            for j in range(0, count_arr[i]):
                self.arr[k] = i
                k += 1


if __name__ == '__main__':
    my_sort = MySort(100000, 99)
    # my_sort.test_sort(my_sort.BubbleSort)
    # my_sort.test_sort(my_sort.SelectSort)
    # my_sort.test_sort(my_sort.InsertSort)
    # my_sort.test_sort(my_sort.QuickSort, 0, my_sort.n - 1)
    # my_sort.test_sort(my_sort.HeapSort)
    # my_sort.test_sort(my_sort.MergeSort, 0, my_sort.n - 1)
    # my_sort.test_sort(my_sort.CountingSort)
