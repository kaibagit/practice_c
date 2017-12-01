#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define    MAXLINE        14096
#define    LISTENQ        20
#define    SERV_PORT    8080

void setnonblocking(int sock)
{
    int opts;
    opts=fcntl(sock,F_GETFL);
    if(opts<0)
    {
        perror("fcntl(sock,GETFL)");
        exit(1);
    }
    opts = opts|O_NONBLOCK;
    if(fcntl(sock,F_SETFL,opts)<0)
    {
        perror("fcntl(sock,SETFL,opts)");
        exit(1);
    }
}

ssize_t socket_send(int sockfd, const char* buffer, size_t buflen)
{
    ssize_t tmp;
    size_t total = buflen;
    const char *p = buffer;

    while(1)
    {
        tmp = send(sockfd, p, total, 0);
        if(tmp < 0)
        {
            // 当send收到信号时,可以继续写,但这里返回-1.
            if(errno == EINTR)
                //return -1;zxd
                continue;

            // 当socket是非阻塞时,如返回此错误,表示写缓冲队列已满,
            // 在这里做延时后再重试.
            if(errno == EAGAIN)
            {
                usleep(1000);
                continue;
            }

            return -1;
        }

        if((size_t)tmp == total)
            return buflen;

        total -= tmp;
        p += tmp;
    }

    return tmp;
}

int main(int argc, char* argv[])
{
    int i, maxi, listenfd, connfd, sockfd, epfd,nfds;
    ssize_t n;
    char BUF[MAXLINE];
    socklen_t clilen;

    //ev用于注册事件,数组用于回传要处理的事件

    struct epoll_event ev,events[20];
    //生成用于处理accept的epoll专用的文件描述符

    epfd=epoll_create(256);
    struct sockaddr_in cliaddr, servaddr;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    setnonblocking(listenfd);
    //设置与要处理的事件相关的文件描述符

    ev.data.fd=listenfd;
    ev.events=EPOLLIN|EPOLLET;
    //注册epoll事件

    epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl (INADDR_ANY);
    servaddr.sin_port = htons (SERV_PORT);
    bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    listen(listenfd, LISTENQ);
    for ( ; ; ) {
        nfds=epoll_wait(epfd,events,20,0);

        if(nfds < 0){
            perror("epoll_wait error");
            exit(1);
        }

        for(i=0;i<nfds;++i)
        {
            if(events[i].data.fd==listenfd)//如果新监测到一个SOCKET用户连接到了绑定的SOCKET端口，建立新的连接。
            {
                connfd = accept(listenfd,(struct sockaddr *)&cliaddr, &clilen);
                if(connfd<0){
                    perror("connfd<0");
                    exit(1);
                }
                setnonblocking(connfd);

                char *str = inet_ntoa(cliaddr.sin_addr);
                printf("accapt a connection from %s\n", str);

                ev.data.fd=connfd;
                ev.events=EPOLLIN|EPOLLET;
                epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&ev);
            }
            else if (events[i].events&EPOLLIN)
                //如果是已经连接的用户，并且收到数据，那么进行读入。
            {
                if ( (sockfd = events[i].data.fd) < 0)
                    continue;

                ssize_t n = 0;
                int nread = 0;
                while((nread= read(sockfd, BUF + n, MAXLINE)) > 0)
                {
                    printf("%d \n",nread);
                    n += nread;
                }//读到EAGAIN，说明读完了

                if(nread == -1 && errno != EAGAIN)
                {
                    perror("readline error");
                    close(sockfd); //关闭一个文件描述符，那么它会从epoll集合中自动删除
                    //描述符关闭后，后面的邋邋邋邋EPOLLOUT设置了，但不起作用了
                    events[i].data.fd = -1;
                }

                //这里要加上判断，nread为0时，说明客户端已经关闭
                //此时，需要关闭描述符，否则在/proc/id/fd下能看到描述符会一直存在
                if(nread == 0)
                {
                    close(sockfd);
                    continue;
                }

                ev.data.fd=sockfd;
                ev.events=EPOLLOUT|EPOLLET;
                //读完后准备写
                epoll_ctl(epfd,EPOLL_CTL_MOD,sockfd,&ev);

            }
            else if(events[i].events&EPOLLOUT) // 如果有数据发送

            {
                sockfd = events[i].data.fd;

                int iRet = socket_send(sockfd, BUF, strlen(BUF) + 1);
                if(iRet == -1 || iRet != strlen(BUF) + 1)
                {
                    perror("write error!");
                }

                ev.data.fd=sockfd;
                ev.events=EPOLLIN|EPOLLET;
                //写完后，这个sockfd准备读
                epoll_ctl(epfd,EPOLL_CTL_MOD,sockfd,&ev);
                //close(sockfd);
            }
        }
    }
    return 0;
}
