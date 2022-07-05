# 编写于2022年06月04日22时34分25秒
# 1、写一个简单的for循环，从1打印到20，横着打为1排
for i in range(1, 21):
    print(i, end=' ')


# 2、写一个say_hello函数打印多次hello并给该函数加备注（具体打印几次依靠传递的参数），然后调用say_hello，同时学会快速查看函数备注快捷键，及如何跳转到函数实现快捷键
def say_hello(times):
    '''这是一个say "hello"函数'''
    for i in range(0, times):
        print('Hello')


say_hello(5)
# 3、写一个模块（命名不要用中文），模块里写3个打印函数，然后另外一个py文件调用该模块，并调用对应模块的函数，同时用一下下面操作
from MyPackages.MyPackage import MyModule1

if __name__ == '__main__':
    MyModule1.print_line(50)  # 调用函数
print(MyModule1.__name__)


# 4、编写代码理解不可变类型，内存中的数据不允许被修改：数字类型 int, bool, float, complex, long(2.x) 字符串 str元组 tuple；可变类型，内存中的数据可以被修改：列表 list字典 dict 集合set

def var_data(i, b, var, f, str1, tuple1, list1):
    i = 3
    b = False
    var = True
    f = 5.3
    # str1 = str1 + ' TGU'  # 不可变类型 改变地址 原数据不变
    # str1=str1.join('TGU')# 改变地址
    str1 = str1.replace('l', 'L')  # 改变地址
    tuple1 = (9, 8, 7)  # 改变地址

    # list1 = [2, 2, 3]# 改变地址
    list1.append(5)  # 列表自带方法不改变地址  原数据同步修改
    list1.pop(0)
    list1.remove(2)
    list1[0] = 4

    print(f'第二部分\ni={i},{id(i)}\nb={b},{id(b)}\nvar={var},{id(var)}\n\
f={f},{id(f)}\nstr1={str1},{id(str1)}\ntuple1={tuple1},{id(tuple1)}\nlist1={list1},{id(list1)}')


i = 2
b = True
var = None
f = 5.2
str1 = 'liu'
tuple1 = (7, 8, 9)
list1 = [1, 2, 3]
print(f'第一部分\ni={i},{id(i)}\nb={b},{id(b)}\nvar={var},{id(var)}\n\
f={f},{id(f)}\nstr1={str1},{id(str1)}\ntuple1={tuple1},{id(tuple1)}\nlist1={list1},{id(list1)}')

print('-' * 30)
var_data(i, b, var, f, str1, tuple1, list1)
print('-' * 30)

print(f'第三部分\ni={i},{id(i)}\nb={b},{id(b)}\nvar={var},{id(var)}\n\
f={f},{id(f)}\nstr1={str1},{id(str1)}\ntuple1={tuple1},{id(tuple1)}\nlist1={list1},{id(list1)}')
# 5、编写代码理解 局部变量与全局变量
num1 = 1
num2 = 2
num3 = 3


def var_local_global(num3):
    global num1
    num1 = -1  # 声明后全局变量
    num2 = -2  # 函数内定义局部变量
    num3 = -3  # 整形传参

    global num4  # 函数内定义全局变量并赋值
    num4 = 4

    print(f'函数内：num1={num1},num2={num2},num3={num3},num4={num4}')  # num1=-1,num2=-2,num3=-3,num4=4


var_local_global(num3)

print(f'函数外：num1={num1},num2={num2},num3={num3},num4={num4}')  # num1=-1,num2=2,num3=3,num4=4
# 6、求两个有序数字列表的公共元素
list1 = [1, 3, 4, 6, 8, 10]
list2 = [2, 3, 5, 7, 10, 11]


def common_element(list1, list2):
    i = j = 0
    while i < len(list1) and j < len(list2):
        if list1[i] < list2[j]:
            i += 1
        elif list1[i] > list2[j]:
            j += 1
        else:
            print(list1[i], end=' ')
            i += 1
            j += 1


common_element(list1, list2)
# 7、给定一个n个整型元素的列表a，其中有一个元素出现次数超过n/2，求这个元素
list3 = [1, 5, 5, 1, 1, 5, 5]


def find_elem(list3):
    candidate = list3[0]
    vote = 0
    for i in range(0, len(list3)):
        if candidate == list3[i]:
            vote += 1
        else:
            vote -= 1
            if vote == 0:  # 相互抵消，为0换人
                vote += 1
                candidate = list3[i]
    return candidate


print(find_elem(list3))
