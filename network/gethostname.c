#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char **argv)
{
    char hostname[32];
    struct hostent *h;

    if( gethostname(hostname,sizeof(hostname)) )
    {
        perror("gethostname");
        return (1);
    }

    if((h=gethostbyname(hostname))==NULL)
     {
         perror("不能得到IP\n");
         exit(1);
     }

    printf("localhost name:%s\n",hostname);

    printf("HostName :%s\n",h->h_name);
    //for(int i = 0; h->h_addr_list[i]; i++) {
        //printf("%s/t", inet_ntoa(*(struct in_addr*)(h->h_addr_list[i])));
    //}
    printf("IP Address :%s\n",inet_ntoa(*((struct in_addr *)h->h_addr)));

    return 0;
}
