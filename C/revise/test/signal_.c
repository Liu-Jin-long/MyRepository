/* test2.c */
// #include <unistd.h>
// #include <stdio.h>
// #include <signal.h>


#include<func.h>
// 信号处理函数使用的全局变量
int count_sig = 0;
void handleSig(int);

void main(){
    printf("main running...\n");
    signal(SIGINT, handleSig);

	// 循环等待信号的到来
    while(1){
        sleep(1);
        //printf("alive.\n");
    }
    printf("done.\n");
}

// 信号处理函数
void handleSig(int sig){
    printf("\n before,signal num is %d\n",sig);
    sleep(3);
    printf("\n after,signal num is %d\n",sig);
}
