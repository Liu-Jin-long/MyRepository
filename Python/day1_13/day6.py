# 编写于2022年06月05日11时22分22秒
dict1 = {('name', 22): 'liu', 'age': 22, 'score': 100.0, 2: 2, 'i': 2}  # TypeError: unhashable type: 'list'
print(dict1)
print(dict1.popitem())
print(dict1)
dict1[('name', 22)] = 'TGU'
print(dict1)
dict2 = dict1.copy()  # 浅copy
print(dict2)
print('-' * 30)
dict1[('name', 22)] = 'LIU'
print(dict2)
for i in dict2:
    print(i)

list1 = [x for x in range(0, 3)]
print(list1)
list2 = [[i for i in range(0, j)] for j in range(0, 5)]
print(list2)
list3 = [j for i in range(10) for j in range(i)]
print(list3)
list4 = [[i for i in range(3)] for j in range(4)]
print(list4)
list5 = [j for j in range(i) for i in range(10)]
print(list5)
list6 = [i for j in range(i) for i in range(10)]
print(list6)

