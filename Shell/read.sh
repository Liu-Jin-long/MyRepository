#!/bin/bash


echo "Please input num1"
#获取标准输入
read num1
echo $num1

#打印提示语句并输入一个内容
read -p "Please input" num2
echo $strnum2

# + - * / % 运算表达式
sum=`expr $num1 + $num2`
echo "sum = $sum"

echo "Please input expression type"
read type
case $type in
+)
    echo "$num1 + $num2 = `expr $num1 + $num2`";;
-)
    echo "$num1 - $num2 = `expr $num1 - $num2`";;
\*)
    echo "$num1 * $num2 = `expr $num1 \* $num2`";;
/)
    echo "$num1 / $num2 = `expr $num1 / $num2`";;
esac
