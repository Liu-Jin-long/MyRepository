#!/bin/bash
echo "欢迎来到猜数字小游戏"
number=$((RANDOM%10+1))
#echo $number
#read -p "请输入你的数字:" guess
while true
do
echo $number
read -p "请输入你的数字:" guess
if [[ $number -eq $guess ]];then
	echo "猜对了！是否继续[y/n]"
	read choice
	if [ $choice = "y" ] || [ $choice = "Y" ];then	
		number=$((RANDOM%10+1))
		echo "继续"
		continue
	else
		echo "退出"
		break
	fi
elif [ $number -gt $guess ];then
	echo "小了"
else
	echo "大了"
fi
done
