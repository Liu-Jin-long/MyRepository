#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
void main(int argc ,char* argv[])
{
	DIR* dir=opendir(argv[1]);
	struct dirent* p;
	while((p=readdir(dir)))
	{
		printf("ino%ld,len=%d,type=%d,name=%s\n",p->d_ino,p->d_reclen,p->d_type,p->d_name);
	}
	closedir(dir);
}
