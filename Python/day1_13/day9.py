# 编写于2022年06月08日18时01分10秒

def symmetric_number(num):
    l = list(str(num))
    i=0
    j=len(l)-1
    while i<=j:
        if l[i]!=l[j]:
            ex=Exception('不是对称数异常')
            raise ex
        i+=1
        j-=1

try:
    num = int(input('请输入一个整数：'))
    symmetric_number(num)
except ValueError as ret:
    print(f'输入不是整数！错误信息：{ret}')
except Exception as e:
    print(e)
    print(e.__traceback__.tb_frame.f_globals['__file__'])
    print(e.__traceback__.tb_lineno)
else:
    print(f'{num}是对称数')
finally:
    print('结束')

try:
    assert 1==0,'前面结果为False，抛出断言异常'
except AssertionError as ret:
    print(ret)

