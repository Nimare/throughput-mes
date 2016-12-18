#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>

#define MAX_CONN_REQ_QUEUE 5

void handle_connection(int socket)
{

}

void nonfatal_error(const char *msg)
{
  perror(msg);
}

void fatal_error(const char *msg)
{
  perror(msg);
  exit(-1);
}

int main(int argc, char *argv[])
{
  int socketfd;
  int newsocketfd;
  struct sockaddr_in server_addr;

  
  socketfd = socket(AF_INET, SOCK_STREAM, 0);
  if (socketfd < 0) {
    fatal_error("ERROR opening socket");
  }

  bzero((char *) &server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(55555);

  if (bind(socketfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
    fatal_error("ERROR on binding");
  }

  if (listen(socketfd, MAX_CONN_REQ_QUEUE) < 0) {
    fatal_error("ERROR listnening on socket");
  }

  struct sockaddr_in client_addr;
  socklen_t client_addr_len = sizeof(client_addr);
  
  while (1) {
    newsocketfd = accept(socketfd, (struct sockaddr *) &client_addr, &client_addr_len);

    if (newsocketfd <0) {
      fatal_error("ERROR on accepting connection");
    }

    int pid = fork();

    if (pid < 0) {
      nonfatal_error("NONFATAL unable to create new thread");
      close(newsocketfd);
    }

    if (pid == 0) {
      close(socketfd);
      handle_connection(newsocketfd);
      exit(0);
    }
    else {
      close(newsocketfd);
    }
    
  }
}
