#include <sys/stat.h>
#include <fcntl.h>
#include <poll.h>

int main(int argc,char *argv[])
{
    struct pollfd fds[2];
    int from_fd = open("/home/dev/c_space/from.txt",O_RDONLY | O_NONBLOCK);
    if(from_fd == -1){
        printf("from_fd \n");
        exit(1);
    }
    int to_fd = open("/home/dev/c_space/to.txt",O_WRONLY | O_NONBLOCK);
    if(to_fd == -1){
        printf("to_fd \n");
        exit(1);
    }
    char buffer[10];
    fds[0].fd = from_fd;fds[0].events = POLLIN;
    fds[1].fd = to_fd;fds[1].events = POLLOUT;
    int loop_times = 1;
    while(1)
    {
        int poll_result = poll(fds,2,60*5);
        printf("=== \n");
        if(poll_result < 0)
        {
            printf("poll_result \n");
            exit(1);
        }else if(poll_result > 0)
        {
            for(int i=0;i< 2;i++)
            {
                int real_read = 0;
                int real_write = 0;
                if(fds[i].fd == from_fd && fds[i].revents == POLLIN)
                {
                    real_read = read(from_fd,buffer,10);
                    if(real_read > 0){
                        printf("loop:%d real_read: %d \n",loop_times,real_read);
                        loop_times++;
                    }else if(real_read == 0){
                        exit(1);
                    }
                }
            }
        }else{
            exit(1);
        }
    }
    exit(0);
}
