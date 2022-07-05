# 编写于2022年06月22日17时20分03秒
class Dog():
    def eat(self):
        print('dog吃骨头')

    def voice(self):
        print('dog汪汪')


class Cat():
    def eat(self):
        print('cat吃🐟')

    def voice(self):
        print('cat喵喵')


class FactoryPattern():
    def __init__(self):
        self.name_dict = {'dog': Dog, 'cat': Cat}

    def create(self, name):
        return self.name_dict[name]()


fp = FactoryPattern()
dog1 = fp.create('dog')
dog1.eat()
dog1.voice()
cat1 = fp.create('cat')
cat1.eat()
cat1.voice()
