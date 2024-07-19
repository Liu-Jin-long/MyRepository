#define _XOPEN_SOURCE  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <sys/time.h>

#include <signal.h>


#define ARGS_CHECK(argc,num) {if(argc!=num){printf("error args\n");return -1;}}
#define ERROR_CHECK(ret,retval,func_name) {if(ret==retval){perror(func_name);return -1;}}