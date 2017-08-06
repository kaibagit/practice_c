#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>

int main(int argc,char *argv[])
{
    struct sockaddr_in addr;
    struct sockaddr_in client;
    addr.sin_family = AF_INET; //IPv4 Internet protocols
    addr.sin_port = htons(8080);  //���ö˿�
    addr.sin_addr.s_addr = inet_addr("0.0.0.0");  //���õ�ַ
    char buf[1024];
    socklen_t lent;
    int opt;
    socklen_t len=sizeof(int);

    int serverfd = socket(AF_INET,SOCK_STREAM,0);
    if (serverfd < 0) {
        printf("socket error \n");
        return -1;
    }

    //设置socket选项
    int nodelay = 1;
    if (setsockopt(serverfd,IPPROTO_TCP,TCP_NODELAY,(void*)&nodelay, sizeof(nodelay)) < 0){
        printf("setsockopt error \n");
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
        memset(&client, 0, sizeof(client));
        lent = sizeof(client);
        int sockfd = accept(serverfd,(struct sockaddr *) &client, &lent);
        if(sockfd < 0){
            printf("accept error %d \n", sockfd);
            return -1;
        }
        int client_port = ntohs(client.sin_port);
        printf("accept success , client ip = %s , port = %d \n",inet_ntoa(client.sin_addr),client_port);
        //printf("clent addr%s porit %d\n",inet_ntop(AF_INET, &clent.sin_addr, buf, sizeof(buf)),ntohs(clent.sin_port));

        // 读取socket选项
        if(getsockopt(sockfd,IPPROTO_TCP,TCP_NODELAY,(char*)&opt,&len) < 0){
            printf("getsockopt error \n");
            return -1;
        };
        printf("TCP_NODELAY Value: %d\n", opt);


        int pid = fork();

        if (pid == 0) {
        //�ӽ���
        close(serverfd);
        handle(sockfd);
        } else if (pid < 0) {
        //error
        close(sockfd);
        } else {
        //������
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
