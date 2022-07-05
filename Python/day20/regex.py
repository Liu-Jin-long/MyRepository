# 编写于2022年06月20日21时10分48秒
import re

# re.match()能够匹配出以xxx开头的字符串
ret = re.match('[0-35-9]Hello', '4Hell0 World')
if ret:
    print(ret.group())

ret = re.match('\s\s\s\shaha', '    haha')
if ret:
    print(ret.group())
list1 = ['bat', 'bit', 'but', 'hat', 'hit', 'hut']
for i in list1:
    print(re.match('[bh][aiu]t', i).group())

ret = re.match('[a-zA-Z][, ][a-zA-Z]+', 'L,smith')
print(ret.group())
ret = re.match('[a-zA-Z][, ][a-zA-Z]+', 'L smith')
print(ret.group())

ret = re.match('[^/]*//www\.\w+\.(com|net|edu)', 'http://www.yahoo.com')
print(ret.group())
ret = re.match('^[a-zA-Z]+', 'liu:12345678910')
print(ret.group())
ret = re.match('^[0-9]+', '12345678910:liu')
print(ret.group())
list2 = ['.\.-=-=', '1-(*76i', 'w_-1', '===end']
for i in list2:
    if re.search('\d+|[a-zA-Z]+', i):
        print(i)

s = 'hello world, now is 2020/7/20 18:48, 现在是2020年7月20日18时48分。'
ret_s = re.sub('年', '/', s)
ret_s = re.sub('月', '/', ret_s)
ret_s = re.sub('日', ' ', ret_s)
ret_s = re.sub('时', ':', ret_s)
ret_s = re.sub('分', '', ret_s)
print(ret_s)
# ?:代表不让findall只匹配分组内容
com = re.compile(r'\d{4}/(?:1[0-2]|[1-9])/[1-3]?[0-9]\s[12]?[0-9]\:[0-6]?[0-9]')
ret = com.findall(ret_s)
print(ret)

list3 = ['xiaoWang@163.com', 'xiongda@qq.com']
for i in list3:
    ret = re.sub('\w+@[^.]*', 'liu@qq', i)
    print(ret)

s = '''
<div>
        <p>岗位职责：</p>
<p>完成推荐算法、数据统计、接口、后台等服务器端相关工作</p>
<p><br></p>
<p>必备要求：</p>
<p>良好的自我驱动力和职业素养，工作积极主动、结果导向</p>
<p>&nbsp;<br></p>
<p>技术要求：</p>
<p>1、一年以上 Python 开发经验，掌握面向对象分析和设计，了解设计模式</p>
<p>2、掌握HTTP协议，熟悉MVC、MVVM等概念以及相关WEB开发框架</p>
<p>3、掌握关系数据库开发设计，掌握 SQL，熟练使用 MySQL/PostgreSQL 中的一种<br></p>
<p>4、掌握NoSQL、MQ，熟练使用对应技术解决方案</p>
<p>5、熟悉 Javascript/CSS/HTML5，JQuery、React、Vue.js</p>
<p>&nbsp;<br></p>
<p>加分项：</p>
<p>大数据，数理统计，机器学习，sklearn，高性能，大并发。</p>

        </div>
'''

ret = re.sub('<[^>]*>|&nbsp;', '', s)  # 支持自定义函数
print(ret)

list4 = ['http://www.interoem.com/messageinfo.asp?id=35',
         'http://3995503.com/class/class09/news_show.asp?id=14',
         'http://lib.wzmc.edu.cn/news/onews.asp?id=769',
         'http://www.zy-ls.com/alfx.asp?newsid=377&id=6',
         'http://www.fincm.com/newslist.asp?id=415'
         ]
for i in list4:
    ret = re.match('[^/]*//[^/]*', i)
    li = re.split('/', ret.group())
    print(li[2])
# 在+号，*号后面加？号，就是非贪婪
ret = re.match(r'.+?(\d+-\d+-\d+-\d+)', s)
print(ret.group(1))

print(re.match(r"aa(\d+)", "aa2343ddd").group(1))

print(re.match(r"aa(\d+?)", "aa2343ddd").group(1))

print(re.match(r"aa(\d+)ddd", "aa2343ddd").group(1))

print(re.match(r"aa(\d+?)ddd", "aa2343ddd").group(1))
