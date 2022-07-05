#include <func.h>
//完成端口的主机及网络字节序的来回转换
//网络字节序是大端方式存储，主机字节序一般都是小段方式存储
int main()
{
    uint16_t hport=0x3456,nport;
    nport=htons(hport);//主机字节序转为网络字节序
    printf("nport=%x\n",nport);
    hport=ntohs(nport);//网络字节序转为主机字节序
    printf("hport=%x\n",hport);

    return 0;
}