#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
void SignChildPsExit(int iSignNo)
{
    int iExitCode;
    pid_t pid = waitpid(-1,&iExitCode,0); //表示等待任何进程，并阻塞。如果换成waitpid(-1,NULL,WNOHANG);跟没有写waitpid效果类似，此时父进程没有阻塞
    printf("SignNo:%d  child %d exit\n",iSignNo,pid);
    if(WIFEXITED(iExitCode))
    {
        printf("Child exited with code %d\n", WEXITSTATUS(iExitCode));
    }
    sleep(5);
    printf("sleep5秒\n");
}
int main()
{
  signal(SIGCHLD, SignChildPsExit);
  printf("Parent process id:%d\n", getpid());
  pid_t iRet = fork();
  if(iRet == 0)
  {
      exit(3);
  }
//   else{
//       sleep(1);
//   }
  //return 0;
}