#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/signal.h>
#include<fcntl.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
//#include <netinet/in.h>

#define MAX_LISTEN_NUM 10
#define MAX_CONNECT_NUM 128
#define LISTEN_PORT 12345

static int g_nCount =0;
void DealSignalIO(int nSig)
{
    printf("nSig (%d) == SIGIO(%d) \n",nSig,SIGIO);
    printf("SignalIO count: %d\n",g_nCount++);
}
int main()
{
    int nRet = 0;
    int nListenSocket = 0,nSubSocket[MAX_CONNECT_NUM];
    nListenSocket = socket(AF_INET,SOCK_STREAM,0);
    if(-1 == nListenSocket)
    {
        perror("socket ");
        return -1;
    }
    struct sockaddr_in MyAddr;
    MyAddr.sin_addr.s_addr = inet_addr("10.10.148.146");
    MyAddr.sin_port = htons(LISTEN_PORT);
    nRet = bind(nListenSocket,(struct sockaddr*)&MyAddr,sizeof(struct sockaddr));
    if(-1 == nRet)
    {
        perror("bind ");
        return -1;
    }
    nRet = listen(nListenSocket,MAX_LISTEN_NUM);
    if(-1 == nRet)
    {
        perror("listen");
        return -1;
    }

    //set the socket not block
    //nRet = fcntl(nListenSocket,F_SETFL,O_NONBLOCK);
    //set the socket asynchronous
    nRet = fcntl(nListenSocket,F_SETFL,O_ASYNC);
    if(-1 == nRet)
    {
        perror("fcntl ");
        return -1;
    }
    //set the SIGIO deal function
    signal(SIGIO,DealSignalIO);
    //set the signal's owner
    nRet = fcntl(nListenSocket,F_SETOWN,getpid());
    if(-1 == nRet)
    {
        perror("fcntl ");
        return -1;
    }

    //accept
    int nLen = 0;
    nRet = accept(nListenSocket,(sockaddr*)&MyAddr,(socklen_t*)&nLen);
    if(-1 == nRet)
    {
        perror("accept ");
    }
    else
    {
        printf("test if accept is blocked \n");
    }


    while(1)
    sleep(2);

    return 0;
}
