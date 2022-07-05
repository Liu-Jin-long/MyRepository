# 编写于2022年06月06日19时36分52秒
# list1 = list('01234')  # ['0', '1', '2', '3', '4', 10]
# list1.append('5')
# print(list1)
# print(list1.count('1'))
# list1.insert(0, -1)
# list1.remove(-1)
# print(list1)
# print(list1)
# list2 = [6, 7]
# list1.extend(list2)
# print(list1)
# list1.pop(6)
# print(list1)
# list1.pop()
# print(list1)
# print(list1.index('1'))
# list1.insert(0, '5')
# print(list1.index('5', 5, 7))
# print(len(list1))
# list1.reverse()
# print(list1)
# list1.sort(reverse=False)  # '<'比较，注意列表内需是同种数据类型  默认升序
# print(list1)
# list2 = list1.copy()
# print(list2)
#
# tuple1 = ('0', '1', '2', '3', '4', '5', '5')
# print(tuple1)
# print(tuple1.index('5'))
# print(tuple1.count('5'))
# print(len(tuple1))
# print(tuple1[5])
#
# print('-' * 50)
# str1 = ''
# print(str1.isspace())
# str1 = 'a1'
# print(str1.isalnum())
# print(str1.isalpha())
# str1 = '1112'
# print(str1.isdecimal())
# print(str1.isdigit())
# str1 = '11a2C'
# print(str1.islower())
# str1 = '11A2C'
# print(str1.isupper())
# str1 = 'abcd efgcd'
# print(str1.startswith('abc'))
# print(str1.endswith('g'))
# print(str1.find('h', 0, len(str1)))  # 未找到返回-1
# print(str1.rfind('cd', 0, len(str1)))
# print(str1.index('cd'))
# print(str1.rindex('a'))  # 未找到报错
# str2 = str1.replace('cd', 'CD', str1.count('cd'))
# print(str2)
# print(str2.capitalize())
# print(str2.title())
# print(str2.swapcase())
# print(str2.lower())
# print(str2.upper())
# str3 = '  abc d e'
# print(str3.ljust(15, 'q'))  # width参数小于原字符串长度时不做改变
# print(str3.rjust(15, 'r'))  # 只进行填充一个同字符
# print(str3.center(15, 'r'))
# poem = ["\t\n  登鹳雀楼",
#  "王之涣" ,
#  "白日依山尽 \t\n",
#  " 黄河入海流",
#  "\n欲穷千里目",
# "更上一层楼\t"]
# for poem_str in poem:
#     print('|%s|' % poem_str.strip().center(10,' '))
# print(str3.rstrip(' d e'))  # 去除最右边字符串，默认去除空格
# print(str3.lstrip('   abc'))
#
# str4 = 'abc \r\ndbe\rf hb\ni bbg'
# print(str4.partition(' '))
# print(str4.rpartition(' '))
# print(str4.split('bg', 3))  # 将原字符串去除传入的子串进行分割，最多从左到右匹配删除num个字串，最多将原字符串分割成num+1个子串
# print(str4.splitlines(True))  # 保留\r\n
# print(str4.splitlines(False))  # 不保留\r\n
# print('-'.join(str3))
#
# str5 = '0123456789'
# print(str5[-1])
# print(str5[::-1])
# print(str5[2:5:2])
# print(str5[2:])
# print(str5[:2])
# print(str5[-2:])
# print(str5[:])

# dict1 = {'name': 'liu', 'age': 22, 'height': 178.5}
# dict1['id'] = 1234
# print(dict1, id(dict1))
# dict2 = dict1.copy()
# print(dict2, id(dict2))
# dict3 = dict1.fromkeys(['1', '2', '3'], None)
# dict3 = dict1.fromkeys(dict1, 2333)  # 取可迭代对象的数据作为新字典的键，第二个参数作为默认值
# print(dict3)
# print(dict1.get('n', 777))  # 取值，未找到返回默认值
# print(dict1.setdefault('n', 666))  # 取值，未找到加入该键值对，返回键值对的值
# print(dict1)
# print(dict1.keys())  # dict_keys(['name', 'age', 'height', 'id', 'n'])
# print(dict1.values())  # dict_values(['liu', 22, 178.5, 1234, 666])
# dict1.update({'age': 1, 'w': 2})  # 将另一字典的键值对更新到dict1里，键相同覆盖
# dict1.update(age=1, height=180.5, q=2)  # 更新添加键值对
# print(dict1)
# print(dict1.pop('nn', 123456))  # 删除键值对，key值必须给出，未找到返回默认值
# print(dict1.popitem())

# set1 = {'A', 'B', 'c', 'ab', 'd'}
# set2 = {'a', 'b', 'c', 'ab', 'd'}
# set1.add('e')
# print(set1)
# set3 = set1.copy()
# print(set3)
# set4 = set1.difference(set2, {'A'}, 'B')  # 返回一个差集
# print(set4)  # {'e'}
# set1.difference_update(set4)  # 在原集合上修改
# print(set1)  # {'d', 'A', 'ab', 'c', 'B'}
# set2.discard('c')  # 所删除元素不在集合中会报错
# print(set2)  # {'d', 'ab', 'a', 'b'}
# set4 = set1.intersection(set2)
# print(set4)  # {'d', 'ab'}
# set1.difference_update(set2)  # 在原集合上移除不重叠的元素
# print(set1)  # {'A', 'c', 'B'}
# set1 = set1.union(set4,{'x','y'},'w','z')
# print(set1)  # {'c', 'd', 'B', 'A', 'ab'}
# set1.update([1, 2])  # 给原集合添加元素 在原集合上并上可迭代类型的数据
# print(set1)  # {1, 2, 'A', 'ab', 'c', 'B', 'd'}
# set1 = set1.symmetric_difference(set2)  # 对称差集
# print(set1)  # {1, 2, 'A', 'b', 'a', 'B', 'c'}
# set1.symmetric_difference_update(set2)
# print(set1)  # {1, 2, 'A', 'd', 'ab', 'B', 'c'}
# set1.remove('b')  # 所删除元素不在集合中会报错
# print(set1)  # {1, 2, 'd', 'ab', 'B', 'A'}
# print(set1.pop())  # 随机移除一个元素
# print(set1)




# TODO 公共接口 reversed sorted等等
tuple1=('a','b','c','d','e')
print(sorted(tuple1,reverse=True))
list1=[5,2,3,6,1]
print(list(reversed(list1)))
list2=[5,3,'e']

print(list(zip(tuple1,list2,list1)))
seasons = ['Spring', 'Summer', 'Fall', 'Winter']
list1=list(enumerate(seasons,5))#默认从0
print(list1)
dict1={}
for i in list1:
    dict1[i[1]]=i[0]
print(dict1)

print(*tuple1)

