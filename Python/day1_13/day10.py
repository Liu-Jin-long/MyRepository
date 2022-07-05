# 编写于2022年06月09日20时18分10秒
# 1、以可读可写打开一个文件，然后写入“人生苦短 我用Python”，关闭文件
import string

f = open('file.txt', 'r+', encoding='utf8')
num = f.write('人生苦短 我用Python')
print(num)
f.close()

import os
import time

file_info = os.stat('file.txt')
print(  # 权限在低九位
    f'size:{file_info.st_size},uid:{file_info.st_uid},mode:{file_info.st_mode},mtime:{time.ctime(file_info.st_mtime)}')

# 2、通过readline依次读取文件每一行并打印
f = open('file.txt', 'r')
while True:
    line = f.readline()  # readline(size) size参数代表要读取的文件内容大小
    if not line:
        break
    print(line, end='')
f.close()
# 3、读取readme文件内容，复制到readme1中
f = open('readme')
f1 = open('readme1', 'w')
while True:
    line = f.readline()  # readline(size) size参数代表要读取的文件内容大小
    if not line:
        break
    f1.write(line)
f.close()
f1.close()
# 4、通过seek从文件开头偏移5个字节，然后把文件剩余内容读取
import os

f = open('file.txt', 'r', encoding='utf8')
f.seek(5, os.SEEK_SET)
text = f.read()
print(text)
f.close()
# 5、练习open的a+，w+模式，感受他们与r+的区别
f = open('file.txt', 'w+', encoding='utf8')
f1 = open('file1.txt', 'a+', encoding='utf8')
text = f.read()  # w和w+w文件存在会被覆盖掉成为空白文件
text1 = f1.read()
print(text)
print('-' * 20)
print(text1)
f.write('LIU')
f1.write('LIU')
f.close()
f1.close()
# 6、练习rb+模式，感受与r+模式的区别
import os

f = open('file.txt', 'r+')
f1 = open('file1.txt', 'rb+')
f.write('abc')
f.write('烫')
f1.write(b'abc')
f1.write('烫'.encode('utf8'))
f.seek(0, os.SEEK_SET)
f1.seek(0, os.SEEK_SET)
text = f.read()
btext = f1.read()
print(text)
print('-' * 20)
print(btext)

# 7、对文件进行改名并删除文件
import os

os.rename('test', 'test1')
os.remove('test1')
# 8、创建文件夹，删除文件夹（均使用相对路径），改变程序执行路径，获取程序当前路径
import os

os.mkdir('./dir')
os.rmdir('./dir')
os.chdir('../MyPackages/')
print(os.getcwd())

import sys


def func():
    print(sys.argv)  # 传参


func()
# 9、实现深度优先遍历
import os


def dfs(path_name, width):
    files_name = os.listdir(path_name)
    for i in files_name:
        print('\t' * width + i)
        cur_path_name = path_name + '\\' + i
        if os.path.isdir((i)):
            dfs(cur_path_name, width + 1)


dfs('..', 0)
# 10、将《The_Holy_Bible.txt》中的标点符号替换成空格，大写字母转换成小写字母并将处理过的文本保存到 “The_Holy_Bible.txt”中。
f = open('file.txt')
f1 = open('file1.txt', 'w')
text = f.read()
for i in text:
    if i in ['.', ',', ';', ':', '?', '[', ']']:
        f1.write(' ')
    elif 'A' <= i <= 'Z':
        f1.write(chr(ord(i) + 32))
    else:
        f1.write(i)
f.close()
f1.close()

# 11、统计” The_Holy_Bible.txt “ 中字符的个数，行数，单词的个数，统计单词的词频并打印输出词频最高的前 10 个单词及其词频（使用第11题处理后的文件来统计）。
f = open('file1.txt', 'r')
line_count = 0
char_count = 0
word_count = 0
dict1 = {}
while True:
    line = f.readline()
    if line:
        line_count += 1
        word = ''
        for i in line:
            if i.isalnum():
                word += i
                char_count += 1
            else:
                # 数字也算字符
                if word.isalpha():
                    word_count += 1
                    if word in dict1:
                        dict1[word] += 1
                    else:
                        dict1[word] = 1
                word = ''
    else:
        break
f.close()
print(len(dict1))
num = 0
for i in dict1:
    num += dict1[i]
print(num)
print(line_count, word_count, char_count)
dict1_order = sorted(dict1.items(), key=lambda i: i[1], reverse=True)
for i in range(10):
    print(f'{dict1_order[i][0]}的词频：{dict1_order[i][1]}')
