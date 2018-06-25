#include <stdio.h>
#include <unistd.h>

#if __APPLE__ && __MACH__
#include <sys/ucontext.h>
#else
#include <ucontext.h>
#endif

typedef std::vector<uthread_t> Thread_vector;

typedef struct schedule_t
{
    ucontext_t main;
    int running_thread;
    Thread_vector threads;

    schedule_t():running_thread(-1){}
}schedule_t;