#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main()
{
  char f[] = "/home/dev/rubyspace/web.xml";
  int fd = open(f,O_RDONLY);
  printf("%d\n",fd);
  if(close(fd) == -1){
    printf("%s","close error");
  }
}
