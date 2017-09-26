#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

#define OFFSET_LENGTH 15000

char buff1[] = "abcdefg";
char buff2[] = "ABCDEFG";
  
int
main(int argc,char** argv)
{
    int fd = 0;

    int buff1Length = strlen(buff1);
    int buff2Length = strlen(buff2);

    char* buff3 = (char*)malloc(buff1Length + buff2Length + OFFSET_LENGTH);
    memset(buff3,2,buff1Length + buff2Length + OFFSET_LENGTH);

    //create hole file
    if((fd = creat("./hole.f",S_IREAD|S_IWRITE)) < 0)
    {
        perror("create file error!");
    }

    printf("fd:%d\n",fd);
    if(write(fd,buff1,buff1Length) != buff1Length)
    {
        perror("write error!");
    }

    if(lseek(fd,OFFSET_LENGTH,SEEK_CUR) == -1)
    {
        perror("lseek error!");
    }

    if(write(fd,buff2,buff2Length) != buff2Length)
    {
        perror("write error!");
    }

    //create nohole file
    if((fd = creat("./nohole.f",S_IREAD|S_IWRITE)) < 0)
    {
        perror("create file error!");
    }

    if(write(fd,buff3,strlen(buff3)) != strlen(buff3))
    {
        perror("write error!");
    }

    free(buff3);

    return 0;
}
