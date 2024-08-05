#!/bin/bash
is_prime(){
	local num=$1
	if [ $num -lt 2 ];then
		return 1
	fi
	for((i=2;i*i<=num;i++));do
		if [ $((num%i)) -eq 0 ];then
			return 1
		fi
	done
	return 0
}

read -p "请输入一个整数:" number
if is_prime $number; then
	echo "$number 是素数"
else	
	echo "$number 不是素数"
fi
