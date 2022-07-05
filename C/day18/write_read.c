#include <func.h>

typedef struct student
{
    int stu_num;
    char name[20];
    int score;
}student;

int main(int argc,char* argv[])
{
    student buff[3]={1001,"zhangsan",88,1025,"lisi",79,1186,"wangwu",96};
    ARGS_CHECK(argc,2)
    int fd;
    fd=open(argv[1],O_RDWR);
    ERROR_CHECK(fd,-1,"open");
    int ret=write(fd,buff,sizeof(buff));
    ERROR_CHECK(ret,-1,"write");
    ret=lseek(fd,0,SEEK_SET);
    ERROR_CHECK(ret,-1,"lseek");
    memset(buff,0,sizeof(buff));
    ret=read(fd,buff,sizeof(buff));
    ERROR_CHECK(ret,-1,"read");
    for(int i=0;i<3;i++)
    {
        printf("%d %s %d\n",buff[i].stu_num,buff[i].name,buff[i].score);
    }
    close(fd);
    return 0;
}