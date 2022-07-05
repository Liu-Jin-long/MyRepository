#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

int which_day_of_this_year(int year,int month,int date)//输入年月日，输出该日期是当年的第几天
{
	int day_number[12] = { 31,28,31,30,31,30,31,31, 30,31,30,31 };
	int sum=0;
	/*int year, month, date;
	scanf("%d%d%d", &year,& month, &date);
	if (month < 1 && month>12 && date<1 && date>day_number[month - 1]) printf("输入非法");*/
	for (int i = 0; i < month-1; i++)
	{
		
		if (i == 1&&(year%4==0&&year%100!=0||year%400==0))
		{
			sum += 29;
			continue;
		}
		sum += day_number[i];
	}
	sum += date;
	return sum;
	/*printf("%d年的第%d天", year, sum);*/

}


int interval_days(int year1,int month1, int date1,int year2,int month2, int date2)
{//输入两个日期（年 月 日 年 月 日）， 输出这两个日期之间差多少天
	int interval = 0;
	/*int year1, month1, date1, year2, month2, date2;
	scanf("%d%d%d", &year1, &month1, &date1, &year2, &month2, &date2);*/
	int day_of_year1 = which_day_of_this_year(year1, month1, date1);
	int day_of_year2 = which_day_of_this_year(year2, month2, date2);
	int max_year = year1 > year2 ? year1 : year2;
	int min_year = year1 + year2 - max_year;
	for (int i = min_year; i < max_year; i++)
	{
		if (i % 4 == 0 && i % 100 != 0 || i % 400 == 0)
		{
			interval += 366;
		}else interval += 365;
	}
	if (year1 = max_year)
	{
		interval = interval + day_of_year1 - day_of_year2;
	}
	else
	{
		interval = interval + day_of_year2 - day_of_year1;
	}
	/*printf("%d年%d月%d日和%d年%d月%d日相差%d天\n", year1, month1, date1, year2, month2, date2, interval);*/
	return interval;

}


int week_day(int year, int month, int date)//输入一个日期，输出该日期是星期几
{

	int year_baseline = 1900, month_baseline = 1, date_baseline = 1, week_day_baseline = 1;
	int interval = interval_days(year, month, date, year_baseline, month_baseline, date_baseline);
	return (interval % 7 + week_day_baseline)>7? interval % 7 + week_day_baseline-7: interval % 7 + week_day_baseline;//1<=(interval % 7 + week_day_baseline)<=13
}

void afer_date(int year, int month, int date, int n)//输入一个日期和一个整数 n，输出从该日期起经过n天以后的日期
{
	//下面思路n先尽可能减去365或366，过于繁琐，已放弃
	//int days = which_day_of_this_year(year, month, date);
	//int common_leap = year % 4 == 0 && year % 100 != 0 || year % 400 == 0 ? 366 : 365;
	//while (n > 0)
	//{
	//	common_leap = year % 4 == 0 && year % 100 != 0 || year % 400 == 0 ? 366 : 365;
	//	if (n >= common_leap)
	//	{
	//		n = n - common_leap;
	//		year++;
	//	}
	//	else break;//n小于当年的天数
	//}
	//while (n > 0)
	//{
	//	if (days + n >= common_leap)//初始闰年 days=365 ，日期
	//	{
	//		n = days + n - common_leap;
	//		year++;
	//		for (month = 1; month <= 12; month++)
	//		{
	//			if(n>= day_number[month-1])
	//		}
	//	}
	//}
	int day_number[12] = { 31,28,31,30,31,30,31,31, 30,31,30,31 };
	n += date;
label:
	while (n > day_number[month - 1])//n减每月天数，尽可能减
	{
		if (month == 2)
		{
			n = n - (year % 4 == 0 && year % 100 != 0 || year % 400 == 0 ? 29 : 28);
			month++;
			goto label;
		}
		if (month == 12)//年份++
		{
			year++;
			n = n - day_number[month - 1];
			month = 1;
			goto label;
		}
		n = n - day_number[month - 1];
		month++;
	}
	date = n;
	printf("之后的日期是%d年%d月%d日", year, month, date);
}


int main()
{
	printf("%d\n", which_day_of_this_year(2022, 1, 3));//year*10000+mon*100+day
	printf("%d\n", interval_days(2022, 1, 3, 2020, 2, 1));
	printf("%d\n", week_day(2022, 2, 4));
	afer_date(2022, 1, 1, 365*3+1);


}