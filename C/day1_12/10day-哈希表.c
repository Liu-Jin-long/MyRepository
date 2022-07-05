#include <stdio.h>

#define MAXKEY 100

//×Ö·û´®¹þÏ£º¯ÊýELFHash

int HashFunction(char* key)
{
	int h = 0, i;
	while (*key)
	{
		h = (h << 4) + *key++;
		i = h & 0xf0000000;
		if(i)
			h ^= i >> 24;
		h &= ~i;
	}
	return h % MAXKEY;
}

void main()
{
	char* s[] = { "daima","hongheishu","bianyiqi","Java","HashFunction" };
	char* Hash_Table[MAXKEY] = { 0 };
	for (int i = 0; i < 5; i++)
	{
		Hash_Table[HashFunction(s[i])] = s[i];
	}

}