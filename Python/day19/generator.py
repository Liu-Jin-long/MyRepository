# 编写于2022年06月20日19时47分02秒
# 简单来说：只要在def中有yield关键字的就称为生成器
# 生成器是一类特殊的迭代器，对于生成器，可按照迭代器的方法使用
L = [i for i in range(5)]
print(L)  # 列表
G = (i for i in range(5))
# print(G)  # 生成器
# for i in G:
#     print(i)
print(next(G))
print(G.__next__())
print(list(G))


def generate():
    a = 0
    for i in range(5):
        temp = yield a
        print(temp)  # 不传参打印None
        a += 1
    return '结束了'


if __name__ == '__main__':
    g = generate()
    print(type(g))
    print(next(g))  # 第一次运行传不了参数或者传None
    # print(list(g))
    # print(g.__next__())
    print(g.send('第一个参数'))
    print(g.send('第二个参数'))
    print(g.send('第三个参数'))

    print(g.__next__())
    try:
        val = g.__next__()
        print(val)
    except StopIteration as e:
        print(f"生成器的函数返回值：{e.value}")
