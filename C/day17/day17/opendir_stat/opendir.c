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
    while((p=readdir(dir)))
    {
        printf("ino=%ld,len=%d,type=%d,name=%s\n",p->d_ino,p->d_reclen,p->d_type,p->d_name);
    }
    closedir(dir);
    return 0;
}

