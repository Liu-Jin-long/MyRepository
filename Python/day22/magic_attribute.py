# 编写于2022年06月23日20时46分13秒
class FOO():
    '''FOO的__dox__'''
    name = 'foo'

    def __init__(self):
        print('类：FOO()时调用')
        self.attr1 = 1
        self.attr2 = 2.22
        self.dict1 = {}

    def func(self):
        return 'FOO的func'

    def __call__(self, *args, **kwargs):
        print('对象：f()时调用')

    def __setitem__(self, key, value):
        print('__setitem__', key, value)
        self.dict1[key] = value

    def __getitem__(self, item):
        print('__getitem__', item)
        return self.dict1[item]

    def __delitem__(self, key):
        print('__delitem__', key)
        del self.dict1[key]


f = FOO()
print(f.__doc__)
print(FOO.__doc__)
print(f.__dict__)
print(FOO.__dict__)
print(f.__module__)
print(FOO.__module__)
print(f.__class__)
print(type(f))
print(FOO.__class__)
print(type(FOO))
f()

f['key1'] = 'liu'
print(f['key1'])
print(f.__dict__)
del f['key1']
