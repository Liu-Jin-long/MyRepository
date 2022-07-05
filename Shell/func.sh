#!/bin/bash
#上面是shebang 释伴

#shell函数里面不需要写形参和返回值类型，默认int类型
#在函数内部，$#和$*代表函数内部的参数和个数，而不是命令的参数和参数个数，函数内部的参数和传参相一致
#调用函数的时候参数传递，不需要添加括号
func()
{
    echo $*
    echo $#
    for i in $*
    do
        echo $i
    done
    return 256
#返回值范围0~255 return%256
}

func hello world 123 liu
echo "return value = $?"

mul()
{
    echo "$num1 * $num2 = `expr $num1 \* $num2`"
}
#传参要和函数内部的参数相同
num1=$1 num2=$2
mul num1 num2