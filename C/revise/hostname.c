#include <func.h>
int main(int argc, char **argv)
{
    ARGS_CHECK(argc, 2)
    struct hostent *p;
    p = gethostbyname(argv[1]);
    printf("%d\n", p->h_addrtype);
    printf("%d\n", p->h_length);
    printf("%s\n", p->h_name);
    for (int i = 0; p->h_aliases[i] != NULL; i++)
    {
        printf("%s\n", p->h_aliases[i]);
    }
    char buf[128];
    for (int i = 0; p->h_addr_list[i] != NULL; i++)
    {
        const char *q = inet_ntop(p->h_addrtype, p->h_addr_list[i], buf, sizeof(buf));
        printf("%s  %s\n", buf, q);
        printf("%p  %p\n", buf, q);
        
    }
    return 0;
}