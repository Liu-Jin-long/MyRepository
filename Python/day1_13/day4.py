# 3.查看变量的各种类型
name = 'liu'
sex = True
age = 22
score = 88.88
list1 = [1, ]
tuple1 = (1,)
set1 = set()
dict1 = {'name': 'liu'}
print(type(name))
print(type(sex))
print(type(age))
print(type(score))
print(type(list1))
print(type(tuple1))
print(type(set1))
print(type(dict1))

# 4.使用input读取变量
num1 = int(input('请输入整数：'))
print(num1)

# 5.练习print的各种变量类型输出
print('姓名：%s' % name)
print('分数：%10.1f' % score)
print('年龄：%05d' % age)


# 6.统计一个整数对应的二进制数的1的个数。输入一个整数（可正可负，负数就按64位去遍历即可）， 输出该整数的二进制包含1的个数
def count1(a):
    """这是一个函数"""
    count = 0
    if 0 == a:
        pass
    elif a > 0:
        while a:
            if a & 1 == 1:
                count += 1
            a >>= 1
    else:
        for i in range(0, 64):
            if a & 1 == 1:
                count += 1
            a >>= 1
    return count


print('-' * 20)
print(count1(10))


# 7.有101个整数，其中有50个数出现了两次，1个数出现了一次， 找出出现了一次的那个数。（大家使用7个数即可）
def appear_once(list1, low, high):
    ret = 0
    for i in range(low, high):
        ret ^= list1[i]
    print(ret)


list1 = [2, 3, 4, 5, 4, 3, 2]
print('-' * 20)
appear_once(list1, 0, len(list1))


# 8.有102个整数，其中有50个数出现了两次，2个数出现了一次， 找出出现了一次的那2个数。（大家使用8个数即可）
def appear_once2(list2):
    n = 0
    for i in list2:
        n ^= i
    n = -n & n  # 所要寻找元素的一个不同位
    low = 0
    high = len(list2) - 1
    temp = list2[0]
    while low < high:  # 模仿快排，将列表元素分为两部分
        while (list2[high] & n) >= 1 and high > low:
            high -= 1
        list2[low] = list2[high]
        while (list2[low] & n) == 0 and high > low:
            low += 1
        list2[high] = list2[low]
    list2[high] = temp
    if temp & n == 0:
        appear_once(list2, 0, high + 1)
        appear_once(list2, high + 1, len(list2))
    elif temp & n >= 1:
        appear_once(list2, 0, high)
        appear_once(list2, high, len(list2))
    else:
        pass


list2 = [2, 3, 4, 5, 4, 3, 2, 6]
print('-' * 20)
appear_once2(list2)
