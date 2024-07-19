#include<stdio.h>
int main(){
    FILE* f=fopen("file","rb+");
    printf("%d\n",feof(f));
    printf("%c %ld\n",fgetc(f),ftell(f));
    printf("1------\n");
    printf("%d\n",feof(f));
    printf("%c %ld\n",fgetc(f),ftell(f));
    printf("2------\n");
    printf("%d\n",feof(f));
    printf("%d %ld\n",fgetc(f),ftell(f));
    printf("3------\n");
    printf("%d\n",feof(f));
}