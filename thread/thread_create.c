#include<stdio.h>
#include<pthread.h>

void *print_pthread_id(void *arg)
{
    printf("current thread id is %u\n",(unsigned)pthread_self());
}

int main(int argc, char * argv[])
{
    pthread_t thread;
    pthread_create(&thread,NULL,print_pthread_id,NULL);
    sleep(1);
    printf("main thread id is %u\n",(unsigned)pthread_self());
    return 0;
}
