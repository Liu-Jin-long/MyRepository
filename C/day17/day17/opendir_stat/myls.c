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
    struct stat buf;
    int ret;
    while((p=readdir(dir)))
    {
        //每次readdir得到一个文件名，把它给stat
        ret=stat(p->d_name,&buf);
        printf("mode=%x %3ld %s  %s %6ld %s %s\n",\
   buf.st_mode,buf.st_nlink,getpwuid(buf.st_uid)->pw_name,getgrgid(buf.st_gid)->gr_name,buf.st_size,ctime(&buf.st_mtime)+4,p->d_name);
    }
    closedir(dir);
    return 0;
}

