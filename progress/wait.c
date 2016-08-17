#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    printf("Hello world!\n");
    int pid = fork();
    if(pid < 0){
        printf("error:%d\n",pid);
    }else if(pid == 0){
        printf("This is son:%d.\n",pid);
        for(int i=0;i<10;i++){
            printf("Son:%d\n",i);
        }
    }else{
        printf("This is parent.\n");
        int childPid = 0;
        while( (childPid = wait(NULL))!= -1 ){
            printf("son:%d finished.\n",childPid);
        }
    }

    return 0;
}
