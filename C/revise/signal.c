#include <func.h>
int signal_hand(int signo)
{
    printf("start %d\n", signo);
    sleep(3);
    printf("end %d\n", signo);
}
int main()
{
    struct sigaction act;
    bzero(&act, sizeof(act));
    act.sa_flags = SA_SIGINFO;
    act.sa_handler = signal_hand;
    int ret = sigaction(SIGINT, &act, NULL);
    ERROR_CHECK(ret, -1, "sigaction");
    ret = sigaction(SIGQUIT, &act, NULL);
    while (1)
        ;
    return 0;
}