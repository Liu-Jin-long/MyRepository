#include <func.h>

int print_dir(char* path,int width)
{
   DIR *dir;
   dir=opendir(path);
   ERROR_CHECK(dir,NULL,"opendir");
   struct dirent *p;
   char buf[1024];
   while((p=readdir(dir)))
   {
       if(!strcmp(".",p->d_name)||!strcmp("..",p->d_name)) continue;
       printf("%*s%s\n",width,"",p->d_name);
       if(p->d_type==4)
       {
           memset(buf,0,sizeof(buf));
           sprintf(buf,"%s%s%s",path,"/",p->d_name);
           print_dir(buf,width+4);
       }


   }
}


int main(int argc,char* argv[])
{
    ARGS_CHECK(argc,2)
    print_dir(argv[1],0);
    return 0;
}