#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include <sys/time.h>

pthread_spinlock_t spinlock;


#define NR_THREAD 2
#define MILLION 1000000L
#define TIMES 100000
#define EXEC_TIMES 1000000

unsigned long long counter = 0;

int little();

int big();

inline int little()
{
    counter++;
}

inline int big()
{
    int j;
    for(j = 0;j<TIMES;j++)
    {
        counter++;
    }
}

void * worker(void* arg)
{
    int i;
    for(i = 0;i<EXEC_TIMES;i++)
    {
        pthread_spin_lock(&spinlock);
        little();
        //big();

        pthread_spin_unlock(&spinlock);

    }

    return NULL;
}
int main()
{
    int i;
    struct timeval tv_start,tv_end;
    unsigned long long interval = 0;
    pthread_spin_init(&spinlock,0);

    pthread_t Tid[NR_THREAD];

    gettimeofday(&tv_start,NULL);
    for(i = 0;i<NR_THREAD;i++)
    {
        if(pthread_create(&Tid[i],NULL,worker,NULL) != 0)
        {
            fprintf(stderr,"pthread create failed when i = %d\n",i);
            return -1;
        }
    }

    for(i = 0;i<NR_THREAD;i++)
    {
        if(pthread_join(Tid[i],NULL))
        {
            fprintf(stderr,"pthread join failed when i = %d\n",i);
            return -2;
        }
    }

    gettimeofday(&tv_end,NULL);
    interval = MILLION*(tv_end.tv_sec - tv_start.tv_sec )
               + (tv_end.tv_usec - tv_start.tv_usec);
    
    fprintf(stderr,"thread num %d spinlock version cost time %llu\n",NR_THREAD,interval);

    return 0;
}