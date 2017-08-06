#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc,char *argv[])
{
    struct sockaddr_in addr;
    struct sockaddr_in clent;
    addr.sin_family = AF_INET; //IPv4 Internet protocols
    addr.sin_port = htons(8080);  //设置端口
    addr.sin_addr.s_addr = inet_addr("0.0.0.0");  //设置地址
    char buf[1024];
    socklen_t lent;

    int serverfd = socket(AF_INET,SOCK_STREAM,0);
    if (serverfd < 0) {
        printf("socket error \n");
        return -1;
    }
    int bind_result = bind(serverfd,(struct sockaddr *)&addr,sizeof(struct sockaddr));
    if(bind_result < 0){
        printf("bind error \n");
        return -1;
    }
    int listen_result = listen(serverfd,128);
    if(listen_result < 0){
        printf("listen error \n");
        return -1;
    }
    for (;;) {
        printf("accept start \n");
        memset(&clent, 0, sizeof(clent));
        lent = sizeof(clent);
        int sockfd = accept(serverfd,(struct sockaddr *) &clent, &lent);
        if(sockfd < 0){
            printf("accept error %d \n", sockfd);
            return -1;
        }
        printf("infor \n");
        //printf("clent addr%s porit %d\n",inet_ntop(AF_INET, &clent.sin_addr, buf, sizeof(buf)),ntohs(clent.sin_port));


        int pid = fork();

        if (pid == 0) {
        //子进程
        close(serverfd);
        handle(sockfd);
        } else if (pid < 0) {
        //error
        close(sockfd);
        } else {
        //父进程
        }
    }

    return 0;
}

int handle(int point) {
    int retn;
    char buf[1024];

    for (;;) {
        retn = read(point, buf, sizeof(buf));
        if (retn < 0) {
            printf("read error \n");
            close(point);
            break;
        } else if (retn == 0) {
            printf("client exit \n");
            close(point);
            break;
        }

        printf("client:%s\n", buf);

        if (strcmp("exit", buf) == 0) {
            printf("exit \n");
            close(point);
            return 0;
        }
    }
    return 0;
}
