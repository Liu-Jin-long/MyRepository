#include <func.h>

int main(int argc,char* argv[])
{
    ARGS_CHECK(argc,2)
    DIR *dir;
    dir=opendir(argv[1]);
    if(NULL==dir)
    {
        perror("opendir");
        return -1;
    }
    
    struct dirent *p;
    struct stat buff;
    int ret;
    int MODE;
    char mode[11]={0};
    char time[256]={0};
    while((p=readdir(dir)))
    {
        ret=stat(p->d_name,&buff);//成功返回0，失败返回-1

        if(p->d_type==4) mode[0]='d';
        else mode[0]='-';
        MODE=(int)buff.st_mode;
        for(int i=9; i>0; i--)
        {
        
            if(MODE%2==1)
            {
                if(i%3==0) mode[i]='x';
                if(i%3==2) mode[i]='w';
                if(i%3==1) mode[i]='r';
            } 
            else mode[i]='-';
            MODE=MODE>>1;
        }
        strcpy(time,ctime(&buff.st_mtime)+4);
        for(int i=strlen(time)-1;i>=0;i--)
        {
            if(time[i]==':')
            {
               time[i]=0;
               break; 
            }
        }
        printf("%s %3ld %s %s %6ld %s %s\n",mode,buff.st_nlink,getpwuid(buff.st_uid)->pw_name,getgrgid(buff.st_gid)->gr_name,buff.st_size,time,p->d_name);

    }
    closedir(dir);
    return 0;
}
