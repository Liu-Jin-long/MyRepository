#!/bin/bash

for i in `seq 1 5`
do
    echo "i = $i ; Hello World"
done


#声明存放整形数据
declare -i n=1
for data in $*
do
    echo "argv[$n]:$data"

#整形
    n=$n+1
done

for i in `seq 1 100`
do

#默认字符串型    
    sum=`expr $sum + $i`
done
echo $sum

declare -i j=1
while test $j -le 100
do
    echo "$j"
    j=$j+1
done

#10的阶乘
j=1 n=1
while test $j -le 10
do
    n=$n*$j
    j=$j+1
done
echo $n

j=1
n=0

#条件为假时执行循环体
until test $j -gt 100
do
    n=`expr $n + $j`
    j=$j+1
done
echo $n


#死循环
while test 0 -eq 0
do
    echo "hello"
done






