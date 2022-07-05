#include <func.h>

void main()
{
    execl("./add","add","3","4",NULL);//execl()其中后缀"l"代表list也就是参数列表
    printf("-----");//执行结果未输出这句
}