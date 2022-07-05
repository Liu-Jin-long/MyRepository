#include "head.h"


//把每一种符号进行token编码，都有对应的数字值
static TKWord KeyWords[] = {
	{TK_PLUS,"+",NULL},
	{TK_MINUS,"-",NULL},
	{TK_STAR,"*",NULL},
	{TK_DIVIDE,"/",NULL},
	{TK_MOD,"%",NULL},
	{TK_EQ,"==",NULL},
	{TK_NEQ,"!=",NULL},
	{TK_LT,"<",NULL},
	{TK_LEQ,"<=",NULL},
	{TK_GT,">",NULL},
	{TK_GEQ,">=",NULL},
	{TK_ASSIGN,"=",NULL},
	{TK_POINTSTO,"->",NULL},
	{TK_DOT,".",NULL},
	{TK_AND,"&",NULL},
	{TK_XOR,"^",NULL},
	{TK_REVERSE,"~",NULL},
	{TK_OPENPA,"(",NULL},
	{TK_CLOSEPA,")",NULL},
	{TK_OPENBR,"[",NULL},
	{TK_CLOSEBR,"]",NULL},
	{TK_BEGIN,"{",NULL},
	{TK_END,"}",NULL},
	{TK_SEMICOLON,";",NULL},
	{TK_COMMA,",",NULL},
	{TK_ELLIPSIS,"...",NULL},
	{TK_REF, "//",NULL},
	{TK_EOF,"End Of File",NULL},

	{TK_CINT,"整形常量",NULL},
	{TK_CFLOAT,"浮点型常量",NULL},
	{TK_CCHAR,"字符常量",NULL},
	{TK_CSTR,"字符串常量",NULL},

	{KW_CHAR,"char",NULL},
	{KW_SHORT,"short",NULL},
	{KW_INT,"int",NULL},
	{KW_VOID,"void",NULL},
	{KW_STRUCT,"struct",NULL},

	{KW_IF,"if",NULL},
	{KW_ELSE,"else",NULL},
	{KW_FOR,"for",NULL},
	{KW_WHILE, "while",NULL},
	{KW_CONTINUE,"continue",NULL},
	{KW_BREAK,"break",NULL},
	{KW_RETURN,"return",NULL},
	{KW_SIZEOF, "sizeof",NULL},
	{0}
};

int main()
{
	memset(tk_HashTable, 0, sizeof(tk_HashTable));
	//打印颜色的handle句柄
	h = GetStdHandle(STD_OUTPUT_HANDLE);
	//将关键字、符号等进行token编码，都有对应的数字值
	int KeyWordsLen = sizeof(KeyWords) / sizeof(TKWord);
	pTKWord phead = NULL, ptail = NULL;//链表头、链表尾
	InitHashTable(KeyWords, KeyWordsLen, tk_HashTable);

	FILE* fp = fopen("file.txt", "r+");
	if (NULL == fp)
	{
		perror("fopen");////判断上一个刚刚执行失败的函数的失败原因
		return -1;
	}

	char buf[200] = { 0 };
	while (fgets(buf, sizeof(buf), fp) != NULL)//每次读一行存在buf中
	{
		splitWords(&phead,&ptail,buf);
		printf("\n");
	}
	fclose(fp);
	return 0;
}