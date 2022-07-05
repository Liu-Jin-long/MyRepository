#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

//
//long main()
//{
//	int i = 500;
//	long long result = 0;
//	int j =1;
//	while (i != 0)
//	{
//		int k = i % 2;
//		result += j * k;
//		j*=10;
//		i = i / 2;
//	}
//	return result;//结果可视为转换后的二进制数
//}
void Decimal_to_Binary(int d)//十进制转二进制
{
	int b[32];
	int i=0;
	if (d == 0) printf("%d", 0);
	while (d != 0)
	{
		b[i++] = d%2;
		d /= 2;

		/*if (d % 2)
		{
			b[i++] = 1;
			d /= 2;
		}
		else
		{
			b[i++] = 0;
			d /= 2;
		}*/
	}
	
	while (i > 0)
	{
		printf("%d", b[i - 1]);
		i--;
	}
}



int Binary_to_Decimal(long b)//二进制转十进制
{
	long d = 0;
	int i = 0;
	while (b)
	{
		d = d + (b % 10)*(1<<i);
		b /= 10;
		i++;
	}
	printf("%d", d);
	return d;
}

void Decimal_to_Hexadecimal(int d)//十进制转十六进制
{
	char b[32];
	int i = 0;
	if (d == 0) printf("%d", 0);
	while (d != 0)
	{
		if(d % 16 > 9)
		{
			b[i++] = d % 16-10+'a';
			d /= 16;
		}
		else
		{
			b[i++] = d % 16+'0';
			d /= 16;
		}
	}

	while (i > 0)
	{
		printf("%c", b[i - 1]);
		i--;
	}
}

int Hexadecimal_to_Decimal(char c[])//二进制转十进制
{
	int d = 0;
	int i = 0, j = 0;
	while (c[i]) i++;
	i--;
	while(i>=0)
	{
		if (c[i] > 47 && c[i] < 58) d = d + (c[i] - '0') * (1 << 4*j);
		if (c[i] > 64 && c[i] < 91) d = d + (c[i] - 'A' + 10) * (1 << 4*j);
		if (c[i] > 96 && c[i] < 123) d = d + (c[i] - 'a' + 10) * (1 << 4*j);
		i--;
		j++;
	}
	printf("%d", d);
	return d;
}







void main()
{
	Decimal_to_Binary(100);
	Binary_to_Decimal(1111011);
	printf("\n");
	Decimal_to_Hexadecimal(123);
	printf("\n");
	char c[10];
	gets(c);
	Hexadecimal_to_Decimal(c);


}