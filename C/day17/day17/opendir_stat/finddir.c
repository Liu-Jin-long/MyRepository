#include <func.h>
//目录深度优先遍历
//width代表空格数
int print_dir(char *path,int width)
{
    DIR *dir;
    dir=opendir(path);
    ERROR_CHECK(dir,NULL,"opendir")
    struct dirent *p;
    char buf[1024];
    while((p=readdir(dir)))
    {
        if(!strcmp(".",p->d_name)||!strcmp("..",p->d_name))
        {
            //如果是.和..什么都不错
        }else{
            printf("%*s%s\n",width,"",p->d_name);
             //判断他是目录，继续把它传给print_dir
            if(p->d_type==4)
            {
                memset(buf,0,sizeof(buf));
                sprintf(buf,"%s%s%s",path,"/",p->d_name);//确保当前进程能够识别该文件夹
                print_dir(buf,width+4);
            }
        }
    }
}

int main(int argc,char* argv[])
{   
    ARGS_CHECK(argc,2)
    print_dir(argv[1],0);
    return 0;
}