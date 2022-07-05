#include <func.h>

int main(int argc,char* argv[])
{   
    ARGS_CHECK(argc,2);
    DIR* dir;
    dir=opendir(argv[1]);
    if(NULL==dir)
    {
        perror("opendir");
        return -1;
    }
    struct dirent *p;
    off_t pos;
    while((p=readdir(dir)))
    {
        printf("ino=%ld,len=%d,type=%d,name=%s\n",p->d_ino,p->d_reclen,p->d_type,p->d_name);
        if(!strcmp(p->d_name,"opendir"))//当发现某个名字是opendir，记录位置
        {
            pos=telldir(dir);
        }
    }
    seekdir(dir,pos);//回到原来保存的位置
    p=readdir(dir);
    printf("ino=%ld,len=%d,type=%d,name=%s\n",p->d_ino,p->d_reclen,p->d_type,p->d_name);
    closedir(dir);
    return 0;
}

