#include "func.h"

int print_dir(char *path, int width)
{
    DIR *dir = opendir(path);
    ERROR_CHECK(dir, NULL, "opendir")
    struct dirent *p;
    char buf[1024];
    while ((p = readdir(dir)))
    {
        if (strcmp(".", p->d_name) == 0 || strcmp("..", p->d_name) == 0)
        {
        }
        else
        {
            printf("%*s%s\n", width, " ", p->d_name);
            sprintf(buf,"%s%s%s",path,"/",p->d_name);
            if (p->d_type == 4)
                print_dir(buf, width + 4);
        }
    }
    closedir(dir);
    return 0;
}
int main(int argv, char *argc[])
{
    ARGS_CHECK(argv, 2)
    print_dir(argc[1], 0);
    return 0;
}