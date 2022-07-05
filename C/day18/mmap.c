#include <func.h>

int main(int argc,char* argv[])
{
    ARGS_CHECK(argc,2)
    int fd=open(argv[1],O_RDWR);
    ERROR_CHECK(fd,-1,"open")
    int ret=write(fd,"hello",5);
    ERROR_CHECK(ret,-1,"write")
    char *p=(char*)mmap(NULL,5,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    ERROR_CHECK(p,(char*)-1,"mmap");
    strcpy(p,"world");
    return 0;

}