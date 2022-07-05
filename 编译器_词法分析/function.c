#include "head.h"

//elf哈希表达式
int elf_hash(char* key) {
	int h = 0, g;
	while (*key) {
		h = (h << 4) + *key++;
		g = h & 0xf0000000;
		if (g)
			h ^= g >> 24;
		h &= ~g;
	}
	return h % MAXKEY;
}

//初始化哈希表
void InitHashTable(pTKWord KeyWords, int KeyWordsLen,TKWord** tk_HashTable)
{
	pTKWord p;
	for (int i = 0; i < KeyWordsLen - 1; i++)
	{		
		p = tk_HashTable[elf_hash(KeyWords[i].word)];
		if (p)//链地址法处理冲突
		{
			while (p)
			{
				if (p->next == NULL) break;
				p = p->next;
			}
			p->next= &KeyWords[i];

		}
		else
		{
			tk_HashTable[elf_hash(KeyWords[i].word)] = &KeyWords[i];
		}
	}
}

//根据token值控制打印颜色
void printColor(char* key, int token)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	if (token >= TK_IDENTV) {//变量名，或者函数名为灰色
		SetConsoleTextAttribute(h, FOREGROUND_INTENSITY);
	}
	else if (token >= KW_CHAR) {//关键字为绿色
		SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	}
	else if (token >= TK_CINT) {//整型常量、浮点型常量、字符常量、字符串常量等为褐色
		SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN);
	}
	else {//运算符为红色
		SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
	}
	if (-1 == key[0]) {
		printf("\n End Of File");
		SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	}
	else {
		printf("%s", key);
	}
}

//链表的尾插法
void Tail_Insert(pTKWord* pphead, pTKWord* pptail, TKWord temp)
{
	pTKWord pnew = (TKWord*)calloc(1, sizeof(TKWord));
	if (pnew)//申请空间要紧接着判断是否成功，否则会报警告
	{
		*pnew = temp;
		pnew->next = NULL;
		if (*pphead == NULL)//插入节点为头节点
		{
			*pphead = pnew;
			*pptail = pnew;
		}
		else
		{
			(*pptail)->next = pnew;
			*pptail = pnew;
		}
		printColor(pnew->word, pnew->token);
	}
	else
	{
		printf("Tail_Insert node calloc failed.");
	}
}

//划分切割每一行，截取下来的小部分存到链表中
void splitWords(pTKWord* pphead, pTKWord* pptail, char* buf)
{
	TKWord temp = { 0 };
	char word[200] = { 0 };
	for (unsigned int i = 0, j; i < strlen(buf); i++)
	{
		j = 0;
		if (isalpha(buf[i]) || '_' == buf[i])//函数名、变量名可以字母和_开头，关键字以字母开头
		{
			while (isalnum(buf[i]) || '_' == buf[i])
			{
				word[j++] = buf[i++];
			}
			i--;

			strcpy(temp.word, word);
			pTKWord cur = tk_HashTable[elf_hash(word)];
			while (cur)//查哈希表取token
			{
				if (!strcmp(cur->word, word)) break;
				cur = cur->next;
			}

			if (cur != NULL )//若是关键字从哈希表中取token
			{
				temp.token = cur->token;
			}
			else//哈希表中不存在的函数名、变量名
			{
				temp.token = TK_IDENTV;
			}
			Tail_Insert(pphead, pptail, temp);
			
		}
		else if (' ' == buf[i])//空格
		{
			printf("%c", buf[i]);
		}
		else if ('"' == buf[i])//字符串常量
		{
			word[j++] = buf[i++];
			while (buf[i] != '"')
			{
				word[j++] = buf[i++];
			}
			word[j] = buf[i];
			strcpy(temp.word, word);
			temp.token = TK_CSTR;
			Tail_Insert(pphead, pptail, temp);

		}
		else if ('\'' == buf[i])//字符常量 注意像左边这种特殊情况 不能只读取'X'三个字符
		{
			word[j++] = buf[i++];
			while (1)
			{
				if ('\'' == buf[i] && buf[i + 1] != '\'') break;
				word[j++] = buf[i++];
			}
			word[j] = buf[i];

			strcpy(temp.word, word);
			temp.token = TK_CCHAR;
			Tail_Insert(pphead, pptail, temp);
		}
		else if ('/' == buf[i] && '/' == buf[i + 1])//注释
		{
			SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);//注释行为绿色
			while (buf[i] != '/n' && buf[i] != '\0')
			{
				word[j++] = buf[i++];
			}
			printf("%s", word);
		}
		else if (isdigit(buf[i]))//整形 浮点型
		{
			while (isdigit(buf[i]))
			{
				word[j++] = buf[i++];
			}
			if ('.' == buf[i])
			{
				word[j++] = buf[i++];
				while (isdigit(buf[i]))
				{
					word[j++] = buf[i++];
				}
				i--;
				
				strcpy(temp.word, word);
				temp.token = TK_CFLOAT;
				Tail_Insert(pphead, pptail, temp);
				
			}
			else
			{
				i--;
				strcpy(temp.word, word);
				temp.token = TK_CINT;
				Tail_Insert(pphead, pptail, temp);
			}
		}
		else if (ispunct(buf[i]))//标点符号或特殊符号（非空格、非数字和非英文字母）
		{
			if ('=' == buf[i] && '=' == buf[i + 1])
			{
				word[j++] = buf[i++];
				word[j++] = buf[i++];
			}
			else if ('!' == buf[i] && '=' == buf[i + 1])
			{
				word[j++] = buf[i++];
				word[j++] = buf[i++];
			}
			else if ('<' == buf[i] && '=' == buf[i + 1])
			{
				word[j++] = buf[i++];
				word[j++] = buf[i++];
			}
			else if ('>' == buf[i] && '=' == buf[i + 1])
			{
				word[j++] = buf[i++];
				word[j++] = buf[i++];
			}
			else if ('-' == buf[i] && '>' == buf[i + 1])
			{
				word[j++] = buf[i++];
				word[j++] = buf[i++];
			}
			else
			{
				word[j++] = buf[i++];
			}
			i--;

			strcpy(temp.word, word);
			pTKWord cur = tk_HashTable[elf_hash(word)];
			while (cur)
			{
				if (!strcmp(cur->word, word)) break;
				cur = cur->next;
			}

			if (cur != NULL && strcmp(cur->word, word) == 0)//若是关键字从哈希表中取token
			{
				temp.token = cur->token;
			}
			else//哈希表中没有存
			{
				temp.token = TK_IDENTV;
			}
			Tail_Insert(pphead, pptail, temp);

		}
		else if ('\t' == buf[i])//tab键打印四个空格
		{
			printf("    ");
		}
		
		memset(&temp, 0, sizeof(temp));
		memset(word, 0, sizeof(word));
	}
}