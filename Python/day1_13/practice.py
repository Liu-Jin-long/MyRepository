print('Hello World!')
ret = 0
for i in range(0, 10):
    j = 1
    if i == 9:
        break
    ret += i
else:  # for正常结束会执行else
    print('跳过')
print(f"result={ret}")

1 and print('花里胡哨作判断')
0 or print('花里胡哨作判断')

if 1:
    x = 2
while 1:
    y = 3
    break

print(f'j={j},x={x},y={y}')


def print_var():
    print(f'j={j},x={x},y={y}')
    global num
    num = 1


print_var()

print(num)


def func(num, *args, **kwargs):
    print(num)  # 1
    print(args)  # (2, 3, 4, 5)
    print(kwargs)  # {'name': '小明', 'age': 18, 'gender': True}


func(1, 2, 3, 4, 5, name="小明", age=18, gender=True)
tuple1 = (2, 3, 4, 5)
dict1 = {'name': '小明', 'age': 18, 'gender': True}
func(2, tuple1, dict1)  # ((2, 3, 4, 5), {'name': '小明', 'age': 18, 'gender': True})\n{}
func(3, *tuple1, **dict1)  # 拆包

