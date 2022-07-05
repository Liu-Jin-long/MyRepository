# 编写于2022年06月13日21时20分08秒
# 二分查找
def BinarySearch(arr, value):
    low = 0
    high = len(arr) - 1
    while low <= high:
        mid = (low + high) // 2
        if arr[mid] < value:
            low = mid + 1
        elif arr[mid] > value:
            high = mid - 1
        else:
            return mid
    return -1


# 哈希查找 链地址法解决冲突
def ELF_Hash(hash_str):
    h = 0
    g = 0
    for i in hash_str:
        h = (h << 4) + ord(i)
        g = h & 0xf0000000
        if g:
            h ^= g >> 24
        h &= ~g
    return h % Hash_Table_Size


class Node:
    def __init__(self, value):
        self.value = value
        self.info = '的其他信息'
        self.next = None


def HashSearch(Hash_Table, value):
    node = Hash_Table[ELF_Hash(value)]
    while node:
        if node.value == value:
            print(f'已从哈希表中找到{value}')
            return node.info
        node = node.next
    return -1


# 位图
def BitMap(arr):
    int_bitmap = 0  # 初始每位都为0
    result = []  # 去重后的结果
    for i in arr:
        if not int_bitmap & 1 << i:
            result.append(i)
            int_bitmap |= 1 << i
    result.sort()
    print(result)
    print(len(result))


if __name__ == '__main__':
    arr = [1, 5, 8, 15, 19, 25, 28, 32, 36, 40]
    # print(BinarySearch(arr, 1))
    str_list = ["xiongda", "lele", "hanmeimei", "wangdao", "fenghua"]
    Hash_Table_Size = 10
    Hash_Table = [Node(None) for i in range(Hash_Table_Size)]
    for i in str_list:
        temp = Hash_Table[ELF_Hash(i)]
        if temp.value == None:
            temp.value = i
        else:
            node = Node(i)
            while temp.next:
                temp = temp.next
            temp.next = node
    print(HashSearch(Hash_Table, 'lele'))
    list1 = [95, 17, 3, 31, 86, 75, 56, 19, 38, 26,
             94, 54, 53, 72, 59, 61, 74, 58, 78, 60,
             64, 43, 52, 90, 84, 19, 92, 2, 71, 12,
             67, 10, 53, 85, 98, 24, 11, 41, 44, 55,
             10, 47, 43, 98, 9, 55, 18, 30, 44, 22,
             48, 15, 87, 28, 47, 18, 92, 3, 38, 87, 59,
             84, 76, 65, 82, 26, 47, 52, 58, 79, 50, 82,
             5, 71, 28, 30, 17, 51, 11, 58, 12, 54, 49, 73,
             24, 46, 99, 94, 93, 70, 12, 33, 19, 67, 62, 74, 61, 89, 91, 51]
    BitMap(list1)
    print('-' * 30)
    print(set(list1))
    print(len(set(list1)))
