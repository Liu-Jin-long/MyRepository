# # 编写于2022年06月08日22时00分44秒
# 1、把今天讲的列表，元组，字典，字符串的所有接口基本功能练习一遍，并用思维导出总结好（不清楚哪些是基础功能，就是照着上课代码练习一遍 ）
# 2、列表、元组，字典的相同点，不同点有哪些，请罗列
# 完成前两题后，后面的题目无论做多少，当天可以先提交一次，后面周三完成了，可以再提交一次
# 3、将元组 (1,2,3) 和集合 {4,5,6} 合并成一个列表。
tuple1 = (1, 2, 3)
set1 = {4, 5, 6}
list1 = list(set1.union(tuple1))
print(list1)
# 4、在列表 [1,2,3,4,5,6] 首尾分别添加整型元素 7 和 0。
list1.insert(0, 7)
list1.insert(6, 0)
# 5、反转列表 [0,1,2,3,4,5,6,7] 。
list1 = [0, 1, 2, 3, 4, 5, 6, 7]
list1.reverse()
print(list1)
# 6、反转列表 [0,1,2,3,4,5,6,7] 后给出中元素 5 的索引号。
print(list1.index((5)))
# 7、分别统计列表 [True,False,0,1,2] 中 True,False,0,1,2的元素个数，发现了什么？
list1 = [True, False, 0, 1, 2, -1, -2]
print(list1.count(True))
print(list1.count(False))
print(list1.count(0))
print(list1.count(1))
print(list1.count(-1))
print(list1.count(2))
print(list1.count(-2))
# 8、从列表 [True,1,0,'x',None,'x',False,2,True] 中删除元素'x'。
list1 = [True, 1, 0, 'x', None, 'x', False, 2, True]
for i in range(list1.count('x')):
    list1.remove('x')
print(list1)
# 9、从列表 [True,1,0,'x',None,'x',False,2,True] 中删除索引号为4的元素。
list1 = [True, 1, 0, 'x', None, 'x', False, 2, True]
list1.pop(4)
print(list1)
# 10、删除列表中索引号为奇数（或偶数）的元素。
list1 = [0, 1, 2, 3, 4, 5, 6, 7, 8]
list1 = list1[::2]
print(list1)
# 11、清空列表中的所有元素。
list1.clear()
print(list1)
# 12、对列表 [3,0,8,5,7] 分别做升序和降序排列。
list1 = [3, 0, 8, 5, 7]
list1.sort()
print(list1)
list1.sort(reverse=True)
print(list1)
# 13、将列表 [3,0,8,5,7] 中大于 5 元素置为1，其余元素置为0。
list1 = [3, 0, 8, 5, 7]
for i in range(len(list1)):
    if list1[i] > 5:
        list1[i] = 1
    else:
        list1[i] = 0
print(list1)
# 14、遍历列表 ['x','y','z']，打印每一个元素及其对应的索引号。
list1 = ['x', 'y', 'z']
for i in range(len(list1)):
    print(f'元素：{list1[i]} 索引：{i}')
# 15、将列表 [0, 1, 2, 3, 4, 5, 6, 7, 8, 9] 拆分为奇数组和偶数组两个列表。
list1 = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
even = list1[::2]
print(even)
odd = list1[1::2]
print(odd)
# 16、分别根据每一行的首元素和尾元素大小对二维列表 [[6, 5], [3, 7], [2, 8]] 排序。
# def sort_func(a,b):
#     if a[0]>b[0]:
#         return a
#     elif a[0]<b[0]:
#         return b
#     else:
#         if a[1] > b[1]:
#             return a
#         elif a[1] < b[1]:
#             return b
#         else:
#             return a
# list1=[[6, 5], [3, 7], [2, 8]]
# sorted(list1,sort_func())

# 17、从列表 [1,4,7,2,5,8] 索引为3的位置开始，依次插入列表 ['x','y','z'] 的所有元素。
list1 = [1, 4, 7, 2, 5, 8]
list1 = list1[:3] + ['x', 'y', 'z'] + list1[3:]
print(list1)
# 18、快速生成由 [5,50) 区间内的整数组成的列表。
list1 = [i for i in range(5, 50)]
print(list1)
# 19、若 a = [1,2,3]，令 b = a，执行 b[0] = 9， a[0]亦被改变。为何？如何避免？----讲了深COPY和浅COPY再做
a = [1, 2, 3]
b = a  # a和b地址相同
import copy

c = copy.deepcopy(a)  # a和c地址不同
print(id(a), id(b), id(c))
b[0] = 9
c[1] = 6
print(a[0], a[1])
# 20、将列表 ['x','y','z'] 和 [1,2,3] 转成 [('x',1),('y',2),('z',3)] 的形式。
list1 = ['x', 'y', 'z']
list2 = [1, 2, 3]
list3 = list(zip(list1, list2))
print(list3)
# 21、以列表形式返回字典 {'Alice': 20, 'Beth': 18, 'Cecil': 21} 中所有的键。
dict1 = {'Alice': 20, 'Beth': 18, 'Cecil': 21}
list1 = [i for i in dict1]
print(list1)
# 22、以列表形式返回字典 {'Alice': 20, 'Beth': 18, 'Cecil': 21} 中所有的值。
list2 = [dict1[i] for i in dict1]
print(list2)
# 23、以列表形式返回字典 {'Alice': 20, 'Beth': 18, 'Cecil': 21} 中所有键值对组成的元组。
list3 = [(i) for i in dict1.items()]
print(list3)
# 24、向字典 {'Alice': 20, 'Beth': 18, 'Cecil': 21} 中追加 'David':19 键值对，更新Cecil的值为17。
# dict1['David']=19
# dict1['Cecil']=17
dict1.update(David=19, Cecil=17)
print(dict1)
# 25、删除字典 {'Alice': 20, 'Beth': 18, 'Cecil': 21} 中的Beth键后，清空该字典。
dict1.pop('Beth')
dict1.clear()
# 26、判断 David 和 Alice 是否在字典 {'Alice': 20, 'Beth': 18, 'Cecil': 21} 中。
dict1 = {'Alice': 20, 'Beth': 18, 'Cecil': 21}
print('David' in dict1)
print('Alice' in dict1)
# 27、遍历字典 {'Alice': 20, 'Beth': 18, 'Cecil': 21}，打印键值对。
for i in dict1.items():
    print(i)
# 28、若 a = dict()，令 b = a，执行 b.update({'x':1})， a亦被改变。为何？如何避免？----讲了深COPY和浅COPY再做
# 29、以列表 ['A','B','C','D','E','F','G','H'] 中的每一个元素为键，默认值都是0，创建一个字典。
list1 = ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H']
dict1 = {}
dict1 = dict1.fromkeys(list1, 0)
print(dict1)
# 30、将二维结构 [['a',1],['b',2]] 和 (('x',3),('y',4)) 转成字典。
list1 = [['a', 1], ['b', 2]]
tuple1 = (('x', 3), ('y', 4))
dict1 = {}
for i in list1:
    dict1.update({i[0]: i[1]})
print(dict1)

# 31、将元组 (1,2) 和 (3,4) 合并成一个元组。
tuple1 = (1, 2)
tuple2 = (3, 4)
tup1e3 = tuple(list(tuple1) + list(tuple2))
print(tup1e3)
# 32、将空间坐标元组 (1,2,3) 的三个元素解包对应到变量 x,y,z。
tuple1 = (1, 2, 3)
x, y, z = tuple1
print(f'x={x},y={y},z={z}')
# 33、返回元组 ('Alice','Beth','Cecil') 中 'Cecil' 元素的索引号。
tuple1 = ('Alice', 'Beth', 'Cecil')
print(tuple1.index('Cecil'))
# 34、返回元组 (2,5,3,2,4) 中元素 2 的个数。
tuple1 = (2, 5, 3, 2, 4)
print(tuple1.count(2))
# 35、判断 'Cecil' 是否在元组 ('Alice','Beth','Cecil') 中。
tuple1 = ('Alice', 'Beth', 'Cecil')
if 'Cecil' in tuple1:
    print(True)
else:
    print(False)
# 36、返回在元组 (2,5,3,7) 索引号为2的位置插入元素 9 之后的新元组。
tuple1 = (2, 5, 3, 7)
list1 = list(tuple1)
list1.insert(2, 9)
tuple2 = tuple(list1)
print(tuple2)
# 37、创建一个空集合，增加 {'x','y','z'} 三个元素。
set1 = set()
set1.update({'x', 'y', 'z'})
print(set1)
# 38、删除集合 {'x','y','z'} 中的 'z' 元素，增j加元素 'w'，然后清空整个集合。
set1 = {'x', 'y', 'z'}
set1.difference_update('z')
set1.update('w')
print(set1)
set1.clear()
# 39、返回集合 {'A','D','B'} 中未出现在集合 {'D','E','C'} 中的元素（差集）。
# set1={'A','D','B'}.difference({'D','E','C'})
set1 = {'A', 'D', 'B'} - {'D', 'E', 'C'}
print(set1)
# 40、返回两个集合 {'A','D','B'} 和 {'D','E','C'} 的并集。
set1 = {'A', 'D', 'B'}.union({'D', 'E', 'C'})
# set1 = {'A', 'D', 'B'} + {'D', 'E', 'C'}  # TypeError: unsupported operand type(s) for +: 'set' and 'set'
print(set1)
# 41、返回两个集合 {'A','D','B'} 和 {'D','E','C'} 的交集。
set1 = {'A', 'D', 'B'}.intersection({'D', 'E', 'C'})
# set1 = {'A', 'D', 'B'} * {'D', 'E', 'C'}
print(set1)
# 42、返回两个集合 {'A','D','B'} 和 {'D','E','C'} 未重复的元素的集合。
set1 = {'A', 'D', 'B'}.symmetric_difference({'D', 'E', 'C'})
print(set1)
# 43、判断两个集合 {'A','D','B'} 和 {'D','E','C'} 是否有重复元素。
if {'A', 'D', 'B'}.intersection({'D', 'E', 'C'}):
    print(True)
else:
    print(False)
# 44、判断集合 {'A','C'} 是否是集合 {'D','C','E','A'} 的子集。
ret = True if {'A', 'C'}.issubset({'D', 'C', 'E', 'A'}) else False
print(ret)
# 45、去除数组 [1,2,5,2,3,4,5,'x',4,'x'] 中的重复元素。
list1 = [1, 2, 5, 2, 3, 4, 5, 'x', 4, 'x']
list1 = list(set(list1))
print(list1)
# 46、返回字符串 'abCdEfg' 的全部大写、全部小写和大下写互换形式。
str1 = 'abCdEfg'
print(str1.upper())
print(str1.lower())
print(str1.swapcase())
# 47、判断字符串 'abCdEfg' 是否首字母大写，字母是否全部小写，字母是否全部大写。
str1 = 'abCdEfg'
print(str1.istitle())
print(str1.islower())
print(str1.isupper())
# 48、返回字符串 'this is python' 首字母大写以及字符串内每个单词首字母大写形式。
print('this is python'.title())
# 49、判断字符串 'this is python' 是否以 'this' 开头，又是否以 'python' 结尾。
ret = True if 'this is python'.startswith('this') and 'this is python'.endswith('python') else False
print(ret)
# 50、返回字符串 'this is python' 中 'is' 的出现次数。
print('this is python'.count('is'))
# 51、返回字符串 'this is python' 中 'is' 首次出现和最后一次出现的位置。
print('this is python'.find('is'))
print('this is python'.rfind('is'))
# 52、将字符串 'this is python' 切片成3个单词。
str1 = 'this is python'.split()
print(str1)
# 53、返回字符串 'blog.csdn.net/xufive/article/details/102946961' 按路径分隔符切片的结果。
str1 = 'blog.csdn.net/xufive/article/details/102946961'.split('/')
print(str1)
# 54、将字符串 '2.72, 5, 7, 3.14' 以半角逗号切片后，再将各个元素转成浮点型或整形。
list1 = '2.72, 5, 7, 3.14'.split(',')
# list1=[eval(i) for i in list1]
# list1=list(map(eval,list1))
# list1 = list(map(lambda i: eval(i), list1))
def func(i): return eval(i)
list1 = list(map(func, list1))
print(list1)
# 55、判断字符串 'adS12K56' 是否完全为字母数字，是否全为数字，是否全为字母？
str1 = 'adS12K56'
print(str1.isalnum())
print(str1.isnumeric())
print(str1.isalpha())
# 56、将字符串 'there is python' 中的 'is' 替换为 'are'。
print('there is python'.replace('is', 'are'))
# 57、清除字符串 '\t python \n' 左侧、右侧，以及左右两侧的空白字符。
print('\t python \n'.lstrip())
print('\t python \n'.rstrip())
print('\t python \n'.strip())
# 58、将三个全英文字符串（比如，'ok', 'hello', 'thank you'）分行打印，实现左对齐、右对齐和居中对齐效果。
s = ('ok', 'hello', 'thank you')
for i in s:
    print(i.ljust(len(s[2])))
for i in s:
    print(i.rjust(len(s[2])))
for i in s:
    print(i.center(len(s[2])))
# 59、将三个字符串（比如，'Hello, 我是David', 'OK, 好', '很高兴认识你'）分行打印，实现左对齐、右对齐和居中效果。
s = ('Hello, 我是David', 'OK, 好', '很高兴认识你')
width = max((len(i) for i in s))
for i in s:
    print(i.ljust(width))
for i in s:
    print(i.rjust(width))
for i in s:
    print(i.center(width))
# 60、将三个字符串 '15', '127', '65535' 左侧补0成同样长度。
print('15'.rjust(len('65535'), str(0)))
print('127'.rjust(len('65535'), str(0)))
print('65535'.rjust(len('65535'), str(0)))
# 61、将列表 ['a','b','c'] 中各个元素用'|'连接成一个字符串。
str1 = '|'.join(['a', 'b', 'c'])
print(str1)
# 62、将字符串 'abc' 相邻的两个字母之间加上半角逗号，生成新的字符串。
str1 = ','.join('abc')
print(str1)
# 63、从键盘输入手机号码，输出形如 'Mobile: 186 6677 7788' 的字符串。
str1=input('请输入电话号码:')
str1=' '.join(('Mobile:',str1[0:3],str1[3:7],str1[7:12]))
print(str1)
# 64、从键盘输入年月日时分秒，输出形如 '2019-05-01 12:00:00' 的字符串。
t=input('请分别输入年月日时分秒：').split()
t='-'.join(t[:3])+' '+':'.join(t[3:6])
print(t)
# 65、给定两个浮点数 3.1415926 和 2.7182818，格式化输出字符串 'pi = 3.1416, e = 2.7183'。
pi = 3.1415926
e = 2.7182818
print('pi:{:.4f},e:{:.4f}'.format(pi, e))
# 66、将 0.00774592 和 356800000 格式化输出为科学计数法字符串。
num1 = 0.00774592
num2 = 356800000
print('{:e} {:e}'.format(num1, num2))
# 67、将列表 [0,1,2,3.14,'x',None,'',list(),{5}] 中各个元素转为布尔型。
list1 = [0, 1, 2, 3.14, 'x', None, '', list(), {5}]
list1 = [bool(i) for i in list1]
print(list1)
# 68、返回字符 'a' 和 'A' 的ASCII编码值。
print(ord('a'), ord('A'))
# 69、返回ASCII编码值为 57 和 122 的字符。
print(chr(57), chr(122))
# 70、将列表 [3,'a',5.2,4,{},9,[]] 中 大于3的整数或浮点数置为1，其余置为0。
list1 = [3, 'a', 5.2, 4, {}, 9, []]
def func(n):
    if isinstance(n, int):
        return 1 if n > 3 else 0
    elif isinstance(n, float):
        return 1 if n > 3 else 0
    else:
        return 0
list1 = list(map(func, list1))
print(list1)
# 71、将二维列表 [[1], ['a','b'], [2.3, 4.5, 6.7]] 转为 一维列表。
list1=[[1], ['a','b'], [2.3, 4.5, 6.7]]
list1=[i for j in list1 for i in j]
print(list1)
# 72、将等长的键列表和值列表转为字典。
list1=range(5)
list2='abcde'
dict1=dict(zip(list1,list2))
print(dict1)
# 73、数字列表求和。
list1=range(5)
print(sum(list1))
