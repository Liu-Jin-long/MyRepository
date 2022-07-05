# 编写于2022年06月14日00时10分39秒
list1 = [[1, 3], [4, 1], [4, 2], [3, 0], [5, 2], [3, 3], [6, 5]]
ret = sorted(list1, key=lambda i: (i[0], -i[1]))  # 对一个字段升序，另一个字段降序
print(ret)
student_tuples = [('john', 'A', 15), ('jane', 'B', 12), ('dave', 'B', 10)]
ret = sorted(student_tuples, key=lambda i: i[2])
print(ret)


class Student:
    def __init__(self, name, grade, age):
        self.name = name
        self.grade = grade
        self.age = age

    def __repr__(self):
        return repr((self.name, self.grade, self.age))


student_objects = [Student('john', 'A', 15), Student('dave', 'B', 10), Student('jane', 'B', 12)]
ret = sorted(student_objects, key=lambda i: i.grade)
print(ret)

from operator import itemgetter, attrgetter

ret = sorted(student_objects, key=attrgetter('grade', 'age'))  # 对象排序
print(ret)
ret = sorted(student_tuples, key=itemgetter(1, -2))
print(ret)

mydict = {'Li': ['M', 7], 'Zhang': ['E', 2], 'Wang': ['P', 3], 'Du': ['C', 2], 'Ma': ['C', 9], 'Zhe': ['H', 7]}
print(
    mydict.items())  # dict_items([('Li', ['M', 7]), ('Zhang', ['E', 2]), ('Wang', ['P', 3]), ('Du', ['C', 2]), ('Ma', ['C', 9]), ('Zhe', ['H', 7])])
ret = sorted(mydict.items(), key=lambda i: i[1][1])
print(ret)

gameresult = [
    {"name": "Bob", "wins": 10, "losses": 3, "rating": 75.00},
    {"name": "David", "wins": 3, "losses": 5, "rating": 57.00},
    {"name": "Carol", "wins": 4, "losses": 5, "rating": 57.00},
    {"name": "Patty", "wins": 9, "losses": 3, "rating": 71.48}]
ret = sorted(gameresult, key=itemgetter('rating', 'name'))  # 列表中嵌入字典排序
print(ret)
