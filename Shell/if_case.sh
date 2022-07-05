#!/bin/bash

#注意不同类型的测试语句  和C语言习惯不同


if test $# -ne 1
then
    echo "input error"
else
    echo "input success"
fi

#注意[ 和 ]两边一定要有空格
if [ $# -eq 1 ]
then
    echo "input success"
else
    echo "input error"
fi


if test -e $1 && test -f $1
then
    echo "hello">>$1
else
    echo "error"
fi


#多分支判断   命令行传两个参数，比字符串大小  注意转义大于小于符号与重定向相区分

if test $1 \< $2
then
    echo "$1 < $2"
elif test $1 \> $2
then
    echo "$1 > $2"
else
    echo "$1 = $2"
fi

case $1 in
1)
    echo "1";;
2)
    echo "2";;
3)
    echo "3";;
*)
    echo "666";;
esac