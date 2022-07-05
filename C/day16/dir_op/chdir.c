#include <unistd.h>
#include <stdio.h>

int main()
{
	char buf[512]={0};
	getcwd(buf,sizeof(buf));//获取当前路径，填入buf
	puts(buf);
	chdir("../");//改变路径到上级目录
	puts(getcwd(NULL,0));//系统给你空间	
	return 0;
}
