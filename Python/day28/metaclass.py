# 编写于2022年06月30日20时03分53秒
import types

print(type)
print(type.__class__)
print(type.__class__.__class__)


class ParentClass():
    desc = '父类属性'


# 实例方法
def echo(self):
    print('This is a method.')
    print(self.attr)


# 静态方法
@staticmethod
def static_method():
    print('This is a static method.')


# 类方法
@classmethod
def class_method(cls):
    print(cls.attr)
    print('This is a class method.')


# 创建了一个名为MyTest的类  可把mytest看成一个变量对MyTest的引用
mytest = type('MyTest', (ParentClass,),
              {'attr': '类属性', 'echo': echo, 'class_method': class_method, 'static_method': static_method})
help(mytest)
print(mytest.__class__)
print(mytest.desc)
mytest.static_method()
mytest.class_method()

print('-' * 50)
test = mytest()
print(test.__class__)  # <class '__main__.MyTest'>
print(test.desc)
test.echo()
test.static_method()
test.class_method()

#############################################################


# 通过metaclass来生成类
import string


def upper_attr_meth(class_name, class_parents, class_attr_method):
    new_attr_method = {}
    for name, value in class_attr_method.items():
        if not name.startswith('__'):
            new_attr_method[name.upper()] = value
    return type(class_name, class_parents, new_attr_method)


class FOO(metaclass=upper_attr_meth):
    class_attr = '类属性'

    @classmethod
    def class_meth(cls):
        # print(cls.CLASS_ATTR)
        print('This is a class method.')


print(hasattr(FOO, 'class_attr'))
print(hasattr(FOO, 'CLASS_ATTR'))
print(FOO.CLASS_ATTR)
print(hasattr(FOO, 'class_meth'))
print(hasattr(FOO, 'CLASS_METH'))
FOO.CLASS_METH()


#####################################################################
class UpperAttrMethodMetaClass(type):
    def __new__(cls, class_name, class_parents, class_attr_method):
        new_attr_method = {}
        for name, value in class_attr_method.items():
            if not name.startswith('__'):
                new_attr_method[name.upper()] = value
        # 方法一:  (尽量不要用)
        # return type(class_name, class_parents, new_attr_method)
        # 方法二：基本的OOP编程，复用type.__new__方法可以保证父类使用metaclass正确
        return type.__new__(cls, class_name, class_parents, new_attr_method)


class Foo(metaclass=UpperAttrMethodMetaClass):
    bar = 'bip'

    @classmethod
    def class_meth(cls):
        # print(cls.CLASS_ATTR)
        print('This is a class method.')


class ChildFoo(Foo):
    pass


print(Foo.BAR)
Foo.CLASS_METH()
print(ChildFoo.BAR)
ChildFoo.CLASS_METH()
