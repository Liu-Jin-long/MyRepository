# 编写于2022年06月23日19时25分03秒
class FOO():
    def __init__(self):
        self.money = 100
        self.__discount = 0.8

    @property  # 类似新建了一个和函数名相同的属性 对属性不同的操作会自动调用不同的函数
    def Mydiscount(self):  # 相关的几个还是函数的函数名必须一致
        return self.__discount

    @Mydiscount.setter
    def Mydiscount(self, value):
        self.__discount = value

    @Mydiscount.deleter
    def Mydiscount(self):
        del self.__discount

    def get_money(self):
        print('get_money')
        return self.money

    def set_money(self, value):
        print('set_money')
        self.money = value

    def del_money(self):
        print('del_money')
        del self.money

    Mymoney = property(get_money, set_money, del_money, 'money的__doc__描述信息')


f = FOO()
print(f.Mymoney)
f.Mymoney = 1000
print(f.Mymoney)
print(FOO.Mymoney.__doc__)
del f.Mymoney

print('*' * 50)
print(f.Mydiscount)
f.Mydiscount = 0.5
print(f.Mydiscount)
