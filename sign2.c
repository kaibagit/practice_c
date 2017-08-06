#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <netinet/in.h>

#define WORKERSIZE 10

int listenfd,connfd;

void sig_iohander(int signo)
{
    int rn;
    char buff[1024];
    puts("get sigio...");
    int connfd = accept(listenfd,NULL,0);
    if(connfd == -1){perror("accept");return;}
    rn = recv(connfd,buff,1023,0);
    if(rn == -1) {perror("recv");return;}
    buff[rn]='\0';
    printf("recv #%s#\n",buff);

    close(connfd);
}

int main(int argc,char*argv[])
{
    struct sockaddr_in cliaddr,servaddr;
    int queuelen=5,i,flag;
    pid_t cpid[WORKERSIZE];

    listenfd = socket(AF_INET,SOCK_STREAM,0);

    signal(SIGIO,sig_iohander);

    flag = fcntl(listenfd,F_GETFL,0);
    flag |= O_NONBLOCK | O_ASYNC;
    fcntl(listenfd,F_SETFL,flag);//将监听套接字设置为非阻塞(非阻塞是因为listenfd发生的连接可能在调用accept的时候，对方已近关闭连接了，这时候如果监听套接字为阻塞，那么进程将长期阻塞直到有新连接完成)，异步IO(O_ASYNC)

    fcntl(listenfd,F_SETOWN,getpid()); //设定listenfd上接收到的中断信号的处理进程ID

    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(8080);

    bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
    listen(listenfd,queuelen);

    while(scanf("%d",&i))//故意让程序阻塞于命令行io，实则等待SIGIO的骚扰~
        puts("jump out of scanf block...");

    return 0;
}
