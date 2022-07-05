#include <func.h>
//完成点分十进制转换为网络字节序
//Internet address.
/*typedef uint32_t in_addr_t;//unsigned long int
struct in_addr
  {
    in_addr_t s_addr;
  };*/
int main(int argc,char* argv[])
{
    ARGS_CHECK(argc,2);
    struct in_addr ip_addr;
    int ret=inet_aton(argv[1],&ip_addr);//成功返回1，失败返回0
    if(ret==0)
    {
        printf("inet_aton failed.\n");
        return -1;
    }
    printf("ip_addr=%x\n",ip_addr.s_addr);//注意返回值
    printf("点分十进制=%s\n",inet_ntoa(ip_addr));
    printf("ip_addr=%x\n",inet_addr(argv[1]));
    return 0;
}
