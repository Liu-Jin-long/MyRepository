#!/bin/bash
#第一行指定命令解析器
echo -e "liu\thello\tworld"
echo -e "liu\nhello\nworld"

#变量字符串类型，等号两边都不能有空格 
a=helloworld
echo $a


# $#相当于C语言的argc（不包含执行脚本名）  $*相当于C语言的argv（不包含执行脚本名,从1开始）
echo $#
echo $*
echo $1
echo $2
echo $3
echo $4

#shell命令要用反引号``括起来
echo "`ls -l`"

#''单引号把括起来的内容看成字符串
echo 'Today is `date`'


touch test
echo hello>test
echo "`cat shell_1.sh`">>test
cat test



