# 编写于2022年07月01日19时40分34秒
# 创建一个实例对象，用创建它的类名当做数据表名，用创建它的类属性对应数据表的字段，当对这个实例对象操作时，能够对应 MySQL 语句
class ModelMetaClass(type):
    def __new__(cls, name, bases, attr):
        mappings = {}  # 表的字段
        for key, value in attr.items():
            # 把类属性都放到mappings字典中
            if isinstance(value, tuple):
                print(f'Find mapping:{key}==>{value}')
                mappings[key] = value
        # 删除已在字典中存储的类属性
        for k in mappings:
            attr.pop(k)
        # 保存属性和列的映射关系
        attr['__mappings__'] = mappings
        # 假设表名和类名一致
        attr['__table__'] = name
        return type.__new__(cls, name, bases, attr)


class User(metaclass=ModelMetaClass):
    # 当指定元类之后，以下的类属性将不在类中，而是在__mappings__属性指定的字典中存储
    uid = ('uid', "int unsigned")
    name = ('username', "varchar(30)")
    email = ('email', "varchar(30)")
    password = ('password', "varchar(30)")

    def __init__(self, **kwargs):
        for name, value in kwargs.items():
            setattr(self, name, value)
        print(self.__dict__)

    def save(self):
        fields = []
        args = []
        for key, value in self.__mappings__.items():
            # fields存放表的字段名
            fields.append(value[0])
            # 避免用户没传某一属性而报错
            args.append(getattr(self, key, None))
        sql = 'insert into %s (%s) values (%s)' % (self.__table__, ','.join(fields),
                                                   ','.join(
                                                       ["'" + i + "'" if isinstance(i, str) else str(i) for i in args]))
        print(sql)


u = User(uid=1, name='liu', email='liu@qq.com', password='123')
print(u.__dict__)
u.save()
