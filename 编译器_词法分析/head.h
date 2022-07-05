#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <Windows.h>

#define MAXKEY 10

typedef enum{
	/* 运算符及分隔符 */
	TK_PLUS,		// + 加号
	TK_MINUS,		// - 减号
	TK_STAR,		// * 星号
	TK_DIVIDE,		// / 除号
	TK_MOD,			// % 求余运算符
	TK_EQ,			// == 等于号
	TK_NEQ,			// != 不等于号
	TK_LT,			// < 小于号
	TK_LEQ,			// <= 小于等于号
	TK_GT,			// > 大于号
	TK_GEQ,			// >= 大于等于号
	TK_ASSIGN,		// = 赋值运算符 
	TK_POINTSTO,	// -> 指向结构体成员运算符
	TK_DOT,			// . 结构体成员运算符
	TK_AND,         // & 地址与运算符
	TK_XOR,			//^异或运算符
	TK_REVERSE,		//~取反运算符
	TK_OPENPA,		// ( 左圆括号
	TK_CLOSEPA,		// ) 右圆括号
	TK_OPENBR,		// [ 左中括号
	TK_CLOSEBR,		// ] 右中括号
	TK_BEGIN,		// { 左大括号
	TK_END,			// } 右大括号
	TK_SEMICOLON,	// ; 分号    
	TK_COMMA,		// , 逗号
	TK_ELLIPSIS,	// ... 省略号
	TK_REF,			// //注释
	TK_EOF,			// 文件结束符

	/* 常量 */
	TK_CINT,		// 整型常量
	TK_CFLOAT,		// 浮点型常量
	TK_CCHAR,		// 字符常量
	TK_CSTR,		// 字符串常量

	/* 关键字 */
	KW_CHAR,		// char关键字
	KW_SHORT,		// short关键字
	KW_INT,			// int关键字
	KW_VOID,		// void关键字  
	KW_STRUCT,		// struct关键字
	KW_IF,			// if关键字
	KW_ELSE,		// else关键字
	KW_FOR,			// for关键字
	KW_WHILE,		// while关键字
	KW_CONTINUE,	// continue关键字
	KW_BREAK,		// break关键字   
	KW_RETURN,		// return关键字
	KW_SIZEOF,		// sizeof关键字

	/* 标识符 */
	TK_IDENTV,//变量名
	TK_IDENTF//函数名
}TokenCode;

typedef struct TKWord//用于保存关键字的链表结点和哈希表拉链法结点用同一种数据结构
{
	int token;
	char word[200];
	struct TKWord* next;
}TKWord,*pTKWord;


TKWord* tk_HashTable[MAXKEY];
HANDLE h;

//初始化哈希表
void InitHashTable(pTKWord KeyWords, int KeyWordsLen, TKWord** Tk_HashTable);
//根据token值控制打印颜色
void printColor(char* key, int token);
//链表的尾插法
void Tail_Insert(pTKWord* pphead, pTKWord* pptail, TKWord temp);
//划分切割每一行，截取下来的小部分存到链表中
void splitWords(pTKWord* pphead, pTKWord* ptail, char* buf);