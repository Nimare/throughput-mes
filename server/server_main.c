#include <sys/types.h>
#include <sys/socket.h> 
#include <stdio.h>
#include <stdlib.h>

void error(const char *msg)
{
  perror(msg);
  exit -1;
}

int main(int argc, char *argv[])
{
  int socketfd;
  
  socketfd = socket(AF_INET, SOCK_STREAM, 0);
  if (socketfd < 0)
  {
    error("ERROR opening socket ");
  }
}
