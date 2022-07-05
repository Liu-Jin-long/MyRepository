# 编写于2022年06月07日09时32分30秒
# 1、函数的简单入参练习
def print_info(name, age):
    print(f'{name},{age}')


print_info('liu', 22)


# 2、函数可变参数 列表，字典入参练习，在函数内改变列表，字典内容后，函数外打印显示发生改变
def variable_parameter(list1, dict1):
    list1 += [4, 5]
    dict1['height'] = 178.5


list2 = [1, 2]
dict2 = {'name': 'liu', 'age': 22}
variable_parameter(list2, dict2)
print(list2, dict2)


# 3、多个缺省参数的传递练习，练习多个缺省参数
def default_parameter(name, age=22, height=178.5):
    print(f'{name},{age},{height}')


default_parameter('liu')
default_parameter('liu', 23)
default_parameter('liu', height=180)


# 4、多值参数练习，元组，字典的传参拆包练习
def multivalued_paramete(name, age=22, *args, **kwargs):
    kwargs['score'] = 100
    print(f'{name},{age},{args},{kwargs}')
    print(type(age))


tuple1 = ('l', 1, 2, 3)
dict1 = {'state': 'haha'}
print(*tuple1)
multivalued_paramete('liu', *tuple1, **dict1)  # 注意拆包后前面有缺省参数


# 5、练习房子-家具类设计，感受类的设计的先后顺序
class Furniture:
    def __init__(self, name, area):
        self.name = name
        self.area = area

    def __str__(self):
        return f'{self.name}占地面积{self.area}'


# class HouseItem:
#     def __init__(self, name, area):
#         self.name = name
#         self.area = area
#
#     def __str__(self):
#         return f'{self.name}占地面积{self.area}'


class House:
    def __init__(self, house_type, area):
        self.house_type = house_type
        self.area = area
        self.remaining_area = area
        self.item_list = []

    def __str__(self):
        return f'房型：{self.house_type}，总面积：{self.area},剩余面积：{self.remaining_area},已容纳家具{self.item_list}'

    def add_item(self, item: Furniture):
        print(f'准备加入{item}')
        if self.remaining_area < item.area:
            print(f'{item}面积太大，{self}剩余空间不足')
        else:
            self.item_list.append(item.name)
            self.remaining_area -= item.area
            print(f'已添加{item}')


bed = Furniture('床', 4)
chest = Furniture('柜子', 2)
table = Furniture('桌子', 1.5)

my_home = House('大别墅', 100)
my_home.add_item(bed)
my_home.add_item(chest)
my_home.add_item(table)
my_home.add_item(table)
print(my_home)


# 6、练习私有属性和私有方法
class Person:
    def __init__(self, name):
        self.name = name
        self.__age = 18

    def __secret(self):
        print(f'年龄：{self.__age}')

    def get_age(self):
        print(self.__age)

    def set_age(self, age):
        self.__age = age


girl = Person('girl')
# print(girl.__age)
print(girl._Person__age)
girl.set_age(20)
girl.get_age()


# 7、练习继承、使用super调用父类方法，多重继承
class Animal1:
    def __init__(self):
        self.name1 = 'Animal1'

    def eat1(self):
        print('吃1')

    def drink(self):
        print('喝1')


class Animal2:
    def __init__(self):
        self.name2 = 'Animal2'

    def eat2(self):
        print('吃2')

    def drink(self):
        print('喝2')


class Dog(Animal1, Animal2):
    def bark(self):
        print('狗叫')
        super().eat2()
        Animal1.eat1(self)


dog = Dog()
print(dog.name1)
print(Dog.__mro__)
dog.eat1()
dog.bark()


# 8、练习类属性，编写类方法，静态方法，并列出他们差异
class Tool:
    """我是帮助Tool.__doc__"""
    count = 0

    def __init__(self, name):
        self.name = name
        Tool.count += 1

    @classmethod  # 类方法应用场景：只需要访问类属性
    def show_tool_count(cls):
        print(f'工具对象的总数{cls.count}')

    @staticmethod  # 静态方法应用场景：既不需要访问实例属性或者调用实例方法，也不需要访问类属性或者调用类方法
    def print_info():
        print('这是类中的静态方法')


tool1 = Tool('斧头')
tool2 = Tool('榔头')
tool3 = Tool('小刀')
print(Tool.count)
print(tool1.count)
Tool.show_tool_count()
print(Tool.__dict__)
print(Tool.__doc__)


# class A:
#     def __init__(self,name):
#         self.name=name
#         print('A初始化')
#     def a(self):
#         print('A的a方法')
#
# class B(A):
#     def __init__(self):
#         self.age=22
#     def a(self):
#         super().a()
#         print('B中调用a方法')
#     def A_init(self):
#         super().__init__('li')
#
# b=B()
# b.a()
# b.A_init()
# list1=B.mro()
# tuple1=B.__mro__
# print(list1)
# print(tuple1)
# print(b.name)

# 9、实现单例模式
class MUsicPlayer:
    instance = None
    init_flag = False

    def __new__(cls, *args, **kwargs):
        if cls.instance is None:
            cls.instance= super().__new__(cls)
            return cls.instance
        else:
            return cls.instance

    def __init__(self,music_name):
        if not MUsicPlayer.init_flag:
            print('初始化:'+music_name)
            MUsicPlayer.init_flag = True


player1 = MUsicPlayer('haha')
print(player1)
print(hex(id(player1)))
del player1
player2 = MUsicPlayer('xixi')
print(player2)
print(hex(id(player2)))
