#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define N 10



void main()
{
	char c[N+2]; //两个50位数相加可能溢出变成51位数
	char d[N+2];
	char result[N + 2] = {'\0'};
	gets(c);
	gets(d);
	int c_len = strlen(c);
	int d_len = strlen(d);
	int len_min = strlen(c) < strlen(d) ? strlen(c) : strlen(d);
	int len_max = c_len + d_len - len_min;
	while (len_min > 0)
	{
		result[len_max-1] = (c[c_len-1] + d[d_len-1] - '0'*2) % 10+'0';
		if (c[c_len-1] + d[d_len-1] - '0' * 2 > 9)
		{
			if(c_len - 1 == 0 && d_len - 1 == 0)
			{
				result[len_max + 1] = '\0';
				for (int i = len_max; i > 0; i--)
				{
					result[i] = result[i - 1];
				}
				result[0] = '1';
				break;
		    }
			if (len_min - 1 == 0 && c_len - 1 == 0)
			{
				d[d_len - 2]++;
				for (int i = 0; i < len_max; i--) 
				{
					result[i] = d[i];
				}
			}
			 if (len_min - 1 == 0 && d_len - 1 == 0)
			{
				c[c_len - 2]++;
				for (int i = 0; i < len_max; i++)
				{
					result[i] = c[i];
				}
			}
			
			c[c_len-2]++;
		}
		c_len--;
		d_len--;
		len_max--;
		len_min--;
	}
	puts(result);

}